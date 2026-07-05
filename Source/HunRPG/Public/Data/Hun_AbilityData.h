// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "HunRPG/Public/System/HunRPG_AbilityTypes.h"
#include "Hun_AbilityData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_AbilityData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Ability")
	EHun_AbilityEffect AbilityEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Ability")
	FHun_AbilityInfo AbilityInfo;
};
