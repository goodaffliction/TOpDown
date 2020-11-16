// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "FunctionLibrary/Types.h"
#include "Engine/World.h"
#include "Game/GameInstanceTD.h"

AMyGameCharacter::AMyGameCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMyGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC)
		{
			FHitResult TraceHitResult;
			myPC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector  CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}
	MovementTick(DeltaSeconds);
}

void AMyGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitWeapon(InitWeaponName);

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

void AMyGameCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);
	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyGameCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyGameCharacter::InputAxisY);

	NewInputComponent->BindAction(TEXT("FireEvent1"), EInputEvent::IE_Pressed, this, &AMyGameCharacter::InputAttackPressed);
	NewInputComponent->BindAction(TEXT("FireEvent1"), EInputEvent::IE_Released, this, &AMyGameCharacter::InputAttackReleased);
	NewInputComponent->BindAction(TEXT("ReloadEvent"), EInputEvent::IE_Released, this, &AMyGameCharacter::TryReloadWeapon);
}

void AMyGameCharacter::InputAxisX(float value)
{
	AxisX = value;
}

void AMyGameCharacter::InputAxisY(float value)
{
	AxisY = value;
}

void AMyGameCharacter::InputAttackPressed()
{
	AttackCharEvent(true);
}

void AMyGameCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void AMyGameCharacter::MovementTick(float DeltaTime)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

	if (MovementState == EMovementState::SprintRun_State)
	{
		FVector myRotationVector = FVector(AxisX, AxisY, 0.0f);
		FRotator myRotator = myRotationVector.ToOrientationRotator();
		SetActorRotation((FQuat(myRotator)));
	}
	else
	{
		APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (myController)
		{
			FHitResult ResultHit;
			//myController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery6, false, ResultHit);
			myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
			float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));

			if (CurrentWeapon)
			{
				FVector Displacement = FVector(0);
				switch (MovementState)
				{
				case EMovementState::Aim_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					CurrentWeapon->ShouldReduceDispersion = true;
					break;
				case EMovementState::AimWalk_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					CurrentWeapon->ShouldReduceDispersion = true;
					break;
				case EMovementState::Walk_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					CurrentWeapon->ShouldReduceDispersion = false;
					break;
				case EMovementState::Run_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					CurrentWeapon->ShouldReduceDispersion = false;
					break;
				case EMovementState::SprintRun_State:
					break;
				}
				CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
			}
		}
	}	
}

void AMyGameCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResSpeed = MovementSpeedInfo.AimSpeedNormal;
		break;
	case EMovementState::AimWalk_State:
		ResSpeed = MovementSpeedInfo.AimSpeedWalk;
		break;
	case EMovementState::Walk_State:
		ResSpeed = MovementSpeedInfo.WalkSpeedNormal;
		break;
	case EMovementState::Run_State:
		ResSpeed = MovementSpeedInfo.RunSpeedNormal;
		break;
	case EMovementState::SprintRun_State:
		ResSpeed = MovementSpeedInfo.RunSpeedSprint;
		break;
	default:
		break;
	}
	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void AMyGameCharacter::ChangeMovementState()
{
		if (!WalkEnable && !SprintRunEnable && !AimEnable)
		{
			MovementState = EMovementState::Run_State;
		}
		else
		{
			if (SprintRunEnable)
			{
				WalkEnable = false;
				AimEnable = false;
				MovementState = EMovementState::SprintRun_State;
			}
			if (WalkEnable && !SprintRunEnable && AimEnable)
			{
				MovementState = EMovementState::AimWalk_State;
			}
			else
			{
				if (WalkEnable && !SprintRunEnable && !AimEnable)
				{
					MovementState = EMovementState::Walk_State;
				}
				else
				{
					if (!WalkEnable && !SprintRunEnable && AimEnable)
					{
						MovementState = EMovementState::Aim_State;

					}
				}
			}

		}
	CharacterUpdate();

	//Weapon State Update
	AWeaponDefault* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon(MovementState);
	}
}

void AMyGameCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponDefault* myWeapon = nullptr;
	myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->SetWeaponStateFire(bIsFiring);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::AttackCharEvent - CurrentWeapon - NULL"));
}

AWeaponDefault* AMyGameCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

UDecalComponent* AMyGameCharacter::GetCursorToWorld()
{
	return CurrentCursor;
}

void AMyGameCharacter::InitWeapon(FName IdWeaponName)
{
	UGameInstanceTD* myGI = Cast<UGameInstanceTD>(GetGameInstance());
	FWeaponInfo myWeaponInfo;
	if (myGI)
	{
		if (myGI->GetWeaponInfoByName(IdWeaponName, myWeaponInfo))
		{
			if (myWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AWeaponDefault* myWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));

				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = myWeapon;
					
					myWeapon->WeaponSetting = myWeaponInfo;
					myWeapon->WeaponInfo.Round = myWeaponInfo.MaxRound;
					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->OnWeaponReloadStart.AddDynamic(this, &AMyGameCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &AMyGameCharacter::WeaponReloadEnd);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon Not Found in table - NULL"));
		}
	}
}

void AMyGameCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);	
}

void AMyGameCharacter::WeaponReloadEnd()
{
	WeaponReloadEnd_BP();
}

void AMyGameCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
}

void AMyGameCharacter::WeaponReloadEnd_BP_Implementation()
{
}

void AMyGameCharacter::TryReloadWeapon()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->GetWeaponRound() <= CurrentWeapon->WeaponSetting.MaxRound)
		{
			CurrentWeapon->InitReload();
			bIsReloading = true;

		}
	}
}





