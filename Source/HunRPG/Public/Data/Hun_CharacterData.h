// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_MobData.h"
#include "Components/Hun_ActorComponent.h"
#include "Engine/DataAsset.h"
#include "HunRPG/Public/System/HunRPG_StateTypes.h"
#include "Hun_CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_CharacterData : public UHun_MobData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Components")
	TArray<TSubclassOf<UHun_ActorComponent>> CharacterComponents;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Move")
	FHun_ActionValue MovementValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Combat")
	FHun_CombatValue CombatValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|AI")
	FHun_MobAI AIValue;
};
