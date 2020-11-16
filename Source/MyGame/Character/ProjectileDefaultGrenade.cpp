// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDefaultGrenade.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AProjectileDefaultGrenade::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectileDefaultGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
}

void AProjectileDefaultGrenade::TimerExplose(float DeltaTime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimeToExplose)
		{
			Explose();
		}
		else
		{
			TimerToExplose += DeltaTime;
		}
	}
}

void AProjectileDefaultGrenade::BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ImpactProjectile();
}

void AProjectileDefaultGrenade::ImpactProjectile()
{
	TimerEnabled = true;
}

void AProjectileDefaultGrenade::Explose()
{
	if (BulletMesh)
	{
		ProjectileSettings.bIsLikeBomp = true;
		TimerEnabled = false;
		if (ProjectileSettings.ExploseFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSettings.ExploseFX, GetActorLocation(), GetActorRotation(), FVector(1.0f));
		}

		if (ProjectileSettings.ExploseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSettings.ExploseSound, GetActorLocation());
		}

		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSettings.ProjectileMaxRadiusDamage, 50, FColor::MakeRandomColor(), false, 0.5f, (uint8)'\000', 1.0f);

		TArray<AActor*> IgnoredActor;
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
			ProjectileSettings.ExploseMaxDamage,
			ProjectileSettings.ProjectileMaxRadiusDamage,
			GetActorLocation(),
			1000.0f,
			2000.0f,
			0.5f,
			NULL, IgnoredActor, nullptr, nullptr);

		this->Destroy();
	}
}