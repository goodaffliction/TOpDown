// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Aim_State	     UMETA(DisplayName = "Aim State"),
	AimWalk_State    UMETA(DisplayName = "AimWalk State"),
	Walk_State       UMETA(DisplayName = "Walk State"),
	Run_State        UMETA(DisplayName = "Run State"),
	SprintRun_State  UMETA(DisplayName = "SprintRun State"),
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
	GENERATED_BODY()
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
		float AimSpeedNormal  = 300.0f; 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
		float AimSpeedWalk    = 100.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
		float WalkSpeedNormal = 200.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
		float RunSpeedNormal  = 600.0f; 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
		float RunSpeedSprint  = 800.0f;
};

USTRUCT (BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		TSubclassOf<class AProjectileDefault> Projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		UStaticMesh* BulletMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		UParticleSystem* BulletFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float ProjectileDamage          = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float ProjectileLifeTime        = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float ProjectileInitSpeed       = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		TMap<TEnumAsByte<EPhysicalSurface>,UMaterialInterface*> HitDecals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		USoundBase* HitSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> HitFXs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		UParticleSystem* ExploseFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		USoundBase* ExploseSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		bool bIsLikeBomp = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float ProjectileMaxRadiusDamage = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
		float ExploseMaxDamage = 100.0f;
};

USTRUCT(BlueprintType)
struct FSleeveInfo
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		TSubclassOf<class ASleeveDefault> Sleeve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float SleeveLifeTime = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float SleeveInitSpeed = 900.0f;
};

USTRUCT(BlueprintType)
struct FClipInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		TSubclassOf<class AWeaponClip> ClipClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float ClipLifeTime = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		float ClipInitSpeed = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
		USoundBase* ReloadSound = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponDispersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Aim_StateDispersionAimMax         = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Aim_StateDispersionAimMin         = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Aim_StateDispersionAimRecoil      = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Aim_StateDispersionReduction      = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float AimWalk_StateDispersionAimMax     = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float AimWalk_StateDispersionAimMin     = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float AimWalk_StateDispersionAimRecoil  = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float AimWalk_StateDispersionReduction  = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Walk_StateDispersionAimMax        = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Walk_StateDispersionAimMin        = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Walk_StateDispersionAimRecoil     = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Walk_StateDispersionReduction     = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Run_StateDispersionAimMax         = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Run_StateDispersionAimMin         = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Run_StateDispersionAimRecoil      = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
		float Run_StateDispersionReduction      = 0.1f;
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Class")
			TSubclassOf<class AWeaponDefault> WeaponClass = nullptr;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "State")
			float             RateOfFire = 0.5f;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "State")
			float             ReloadTime = 2.0f;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "State")
			int               MaxRound =10;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "State")
			int               NumberByProjectileShot = 1;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Dispersion")
			FWeaponDispersion DispersionWeapon;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon Sleeve")
			FSleeveInfo       SleeveSetting;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon Clip")
			FClipInfo         WeaponClipSetting;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Sound")
			USoundBase*       SoundFireWeapon   = nullptr;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Sound")
			USoundBase*       SoundReloadWeapon = nullptr;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FX")
			UParticleSystem*  EffectFireWeapon  = nullptr;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Projectile")
			FProjectileInfo   ProjectileSettings;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Trace")
			float             WeaponDamage  = 20.0f;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Trace")
			float             DistanceTrace = 2000.0f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
			TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> HitDecals;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
			USoundBase* HitSound = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
			TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> HitFX;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "HitEffect")
			UDecalComponent*  DecalOnHit  = nullptr;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Anim")
			UAnimMontage*     AnimCharFire   = nullptr;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Anim")
			UAnimMontage*     AnimCharReload = nullptr;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
			UStaticMesh*      MagazineDrop  = nullptr;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
			UStaticMesh*      SheeveBullets = nullptr;
};

USTRUCT(BlueprintType)
struct FAdditionalWeaponInfo
{
	GENERATED_BODY()
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon Stats")
		int32 Round =10;
};

UCLASS()
class MYGAME_API UTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};