// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceTD.h"

bool UGameInstanceTD::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
    bool bIsFind = false;
    FWeaponInfo* WeaponInfoRow;

    if (WeaponInfoTable)
    {
        WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);
        if (WeaponInfoRow)
        {
            bIsFind = true;
            OutInfo = *WeaponInfoRow;
        }
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("UMyGameGameInstance::GetWeaponInfoByName - WeaponTable - NULL"))
    }
        return bIsFind;
}
