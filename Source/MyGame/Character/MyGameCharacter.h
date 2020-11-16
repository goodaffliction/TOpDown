// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponClip.h"
#include "WeaponDefault.h"
#include "GameFramework/Character.h"
#include "FunctionLibrary/Types.h"
#include "MyGameCharacter.generated.h"


UCLASS(Blueprintable)
class AMyGameCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	AMyGameCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	public:
		//Cursor
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cursor")
			UMaterialInterface* CursorMaterial = nullptr;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cursor")
			FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

		//Movement
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
			EMovementState MovementState = EMovementState::Run_State;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
			FCharacterSpeed MovementSpeedInfo;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
			bool SprintRunEnable = false;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
			bool WalkEnable = false;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
			bool AimEnable = false;
		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ReloadLogic")
			bool bIsReloading = false;

		//Weapon	
		UFUNCTION(BlueprintCallable)
			AWeaponDefault* GetCurrentWeapon();
			AWeaponDefault* CurrentWeapon = nullptr;

		//for demo 
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
			FName InitWeaponName;
		UDecalComponent* CurrentCursor = nullptr;

		//Inputs
		UFUNCTION()
			void InputAxisX(float value);
		UFUNCTION()
			void InputAxisY(float value);
		UFUNCTION()
			void InputAttackPressed();
		UFUNCTION()
			void InputAttackReleased();

			float AxisX = 0.0f;
			float AxisY = 0.0f;

		//Tick function
		UFUNCTION()
		void MovementTick(float DeltaTime);

		//Functions
		UFUNCTION(BlueprintCallable)
			void CharacterUpdate();
		UFUNCTION(BlueprintCallable)
			void ChangeMovementState();
		UFUNCTION(BlueprintCallable)
			void AttackCharEvent(bool bIsFiring);
		UFUNCTION(BlueprintCallable)
			void TryReloadWeapon();

		UFUNCTION(BlueprintCallable)
			UDecalComponent* GetCursorToWorld();

		UFUNCTION(BlueprintCallable)
			void InitWeapon(FName IdWeapon);

		UFUNCTION()
			void WeaponReloadStart(UAnimMontage* Anim);
		UFUNCTION()
			void WeaponReloadEnd();

		UFUNCTION(BlueprintNativeEvent)
			void WeaponReloadStart_BP(UAnimMontage* Anim);
		UFUNCTION(BlueprintNativeEvent)
			void WeaponReloadEnd_BP();

		
		
};