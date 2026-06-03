// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hun_ComponentsData.generated.h"

/**
 * 
 */
class UHun_ActorComponent;

UCLASS()
class HUNRPG_API UHun_ComponentsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Components")
	TArray<TSubclassOf<UHun_ActorComponent>> CharacterComponents;
};
