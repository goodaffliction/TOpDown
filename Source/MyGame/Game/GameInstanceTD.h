// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FunctionLibrary/Types.h"
#include "Engine/DataTable.h"
#include "Character/WeaponDefault.h"
#include "GameInstanceTD.generated.h"

UCLASS()
class MYGAME_API UGameInstanceTD : public UGameInstance
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "WeaponSettings")
		UDataTable* WeaponInfoTable = nullptr;
	UFUNCTION(BlueprintCallable)
		bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
};
