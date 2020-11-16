// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponClip.h"
#include "Components/ArrowComponent.h"
#include "ProjectileDefault.h"
#include "SleeveDefault.h"
#include "FunctionLibrary/Types.h"
#include "WeaponDefault.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnWeaponReloadStart, UAnimMontage*, Anim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE (FOnWeaponReloadEnd);
UCLASS()
class MYGAME_API AWeaponDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDefault();

	FOnWeaponReloadStart OnWeaponReloadStart;
	FOnWeaponReloadEnd   OnWeaponReloadEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USceneComponent*        SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent*   StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UArrowComponent*        ShootLocation = nullptr;

	UPROPERTY()
		FWeaponInfo WeaponSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		FAdditionalWeaponInfo WeaponInfo;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireTick(float DeltaTime);
	void ReloadTick(float DeltaTime);
	void DispersionTick(float DeltaTick);

	void WeaponInit();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FireLogic")
		bool WeaponFiring    = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ReloadLogic")
		bool WeaponReloading = false;

	UFUNCTION(BlueprintCallable)
	void SetWeaponStateFire(bool bIsFire);

	bool CheckWeaponCanFire();

	FProjectileInfo GetProjectile();
	FSleeveInfo GetSleeve();
	

	void Fire();

	void UpdateStateWeapon(EMovementState NewMovementState);
	void ChangeDispersion();
	float GetCurrentDispersion() const;
	FVector ApplyDispersionToShoot(FVector DirectionShoot)const;
	FVector GetFireEndLocation()const;
	FVector ShootEndLocation = FVector(0);
	int8 GetNumberProjectileByShot() const;

	float FireTimer = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ReloadLogic")
	float ReloadTimer = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ReloadLogic")
	float ReloadTime = 0.0f;

	//flags
	bool BlockFire = false;
	//Dispersion
	bool ShouldReduceDispersion = false;
	float CurrentDispersion = 0.0f;
	float CurrentDispersionMax = 1.0f;
	float CurrentDispersionMin = 0.1f;
	float CurrentDispersionRecoil = 0.1f;
	float CurrentDispersionReduction = 0.1f;

	UFUNCTION(BlueprintCallable)
	int32 GetWeaponRound();
	void InitReload();
	void FinishReload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool ShowDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		float SizeVectorToChangeShootDirectionLogic = 100.0f;

	UFUNCTION(BlueprintCallable)
		AWeaponClip* GetCurrentClip();
	FClipInfo GetClip();
		AWeaponClip* CurrentClip = nullptr;

	UFUNCTION()
		void TraceHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
