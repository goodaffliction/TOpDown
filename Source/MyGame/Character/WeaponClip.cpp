// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponClip.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeaponClip::AWeaponClip()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ClipCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	//ClipCollisionSphere->SetSphereRadius(16.f);
	//ClipCollisionSphere->bReturnMaterialOnMove = true;
	//ClipCollisionSphere->SetCanEverAffectNavigation(false);

	RootComponent = ClipMesh;
	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	ClipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Clip Mesh"));
	ClipMesh->SetupAttachment(RootComponent);
	ClipMesh->SetCanEverAffectNavigation(false);

	//ClipMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Weapon Clip Movement"));
	//ClipMovementComponent->UpdatedComponent = RootComponent;
	//ClipMovementComponent->InitialSpeed = 100.0f;
	//ClipMovementComponent->MaxSpeed = 100.0f;

	//ClipMovementComponent->bRotationFollowsVelocity = true;
	//ClipMovementComponent->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AWeaponClip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponClip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponClip::InitClip(FClipInfo InitParam)
{
	//ClipMovementComponent->InitialSpeed = InitParam.ClipInitSpeed;
	//ClipMovementComponent->MaxSpeed = InitParam.ClipInitSpeed;
	this->SetLifeSpan(InitParam.ClipLifeTime);
	ClipDefaultInfo = InitParam;
}

