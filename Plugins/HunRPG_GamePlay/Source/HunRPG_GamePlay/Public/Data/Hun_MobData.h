// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hun_MobData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_GAMEPLAY_API UHun_MobData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Stat")
	int MaxHealthPoint = 100;
};
