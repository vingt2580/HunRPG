// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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
	float WalkSpeed = 600.f;
};
