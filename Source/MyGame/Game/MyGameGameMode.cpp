// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyGameGameMode.h"
#include "MyGamePlayerController.h"
#include "Character/MyGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyGameGameMode::AMyGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Character/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}