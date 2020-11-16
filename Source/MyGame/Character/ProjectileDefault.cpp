// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDefault.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileDefault::AProjectileDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	BulletCollisionSphere->SetSphereRadius(16.f);
	BulletCollisionSphere->bReturnMaterialOnMove = true; //hit event, return physMaterial
	BulletCollisionSphere->SetCanEverAffectNavigation(false);

	RootComponent = BulletCollisionSphere;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Projectile Mesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet FX"));
	BulletFX->SetupAttachment(RootComponent);

	//BulletAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Bullet Audio"));
	//BulletAudio->SetupAttachment(RootComponent);

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet ProjectileMovement"));
	BulletProjectileMovement->UpdatedComponent = RootComponent;
	BulletProjectileMovement->InitialSpeed = 1.0f;
	BulletProjectileMovement->MaxSpeed = 0.0f;

	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AProjectileDefault::BeginPlay()
{
	Super::BeginPlay();
	BulletCollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereHit);
	BulletCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereBeginOverlap);
	BulletCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereEndOverlap);

	FVector SpawnLocation = RootComponent->GetComponentLocation();
}

// Called every frame
void AProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileDefault::InitProjectile(FProjectileInfo InitParam)
{
	BulletProjectileMovement->InitialSpeed = InitParam.ProjectileInitSpeed;
	BulletProjectileMovement->MaxSpeed = InitParam.ProjectileInitSpeed;
	this->SetLifeSpan(InitParam.ProjectileLifeTime);
	ProjectileSettings = InitParam;
}

void AProjectileDefault::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface mySurfaceType = UGameplayStatics::GetSurfaceType(Hit);
		if (ProjectileSettings.HitDecals.Contains(mySurfaceType))
		{
			UMaterialInterface* myMaterial = ProjectileSettings.HitDecals[mySurfaceType];
			if (myMaterial && OtherComp)
			{
				UGameplayStatics::SpawnDecalAttached(myMaterial, FVector(20.0f), OtherComp, NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
			}
		}
		if (ProjectileSettings.HitFXs.Contains(mySurfaceType))
		{
			UParticleSystem* myParticle = ProjectileSettings.HitFXs[mySurfaceType];
				if (myParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myParticle, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.0f)));
				}
		}
		if (ProjectileSettings.HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSettings.HitSound, Hit.ImpactPoint);
		}
	}
	UGameplayStatics::ApplyDamage(OtherActor, ProjectileSettings.ProjectileDamage*GetKoef(), GetInstigatorController(), this, NULL);
	ImpactProjectile();
}

void AProjectileDefault::BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectileDefault::BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	this->Destroy();
}

FVector AProjectileDefault::GetFireEndLocation() const
{
	bool bShootDirection = false;
	FVector EndLocation = FVector(0.0f);
	EndLocation = RootComponent->GetComponentLocation();
	return EndLocation;
}

float AProjectileDefault::GetKoef()
{
	Lenght = (GetFireEndLocation() - SpawnLocation).Size();
	KoefOfDamage = 0.003*Lenght;
	return KoefOfDamage;
}

void AProjectileDefault::ImpactProjectile()
{
	this->Destroy();
}