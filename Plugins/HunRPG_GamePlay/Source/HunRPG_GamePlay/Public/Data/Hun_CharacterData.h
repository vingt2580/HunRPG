// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HunRPG_Core/Public/System/HunRPG_StateTypes.h"
#include "Hun_CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_GAMEPLAY_API UHun_CharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Components")
	TArray<TSubclassOf<UActorComponent>> CharacterComponents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Name")
	FString Name = "CharacterName";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Move")
	FHun_ActionValue MovementValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Combat")
	FHun_CombatValue CombatValue;
};
