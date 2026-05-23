// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hun_MobData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_MobData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Stat")
	int MaxHealthPoint = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Animation")
	class UAnimMontage* HitReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Animation")
	class UAnimMontage* DeathMontage;
};
