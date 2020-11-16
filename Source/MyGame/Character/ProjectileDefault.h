// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"

#include "FunctionLibrary/Types.h"
#include "ProjectileDefault.generated.h"

UCLASS()
class MYGAME_API AProjectileDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileDefault();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
		class UStaticMeshComponent*         BulletMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
		class UParticleSystemComponent*		BulletFX = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
		class USphereComponent*             BulletCollisionSphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
		class UProjectileMovementComponent* BulletProjectileMovement;
	

	FProjectileInfo ProjectileSettings;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector GetFireEndLocation()const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float KoefOfDamage = 0.0f;
	float KoefOfLenght = 100.0f;
	FVector ShootEndLocation = FVector(0);
	FVector SpawnLocation = FVector(0);

	float Lenght = 0.0f;

	bool bIsLikeBomp = false;

	float GetKoef();

	void InitProjectile(FProjectileInfo InitParam);

	UFUNCTION()
		virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void ImpactProjectile();
};
