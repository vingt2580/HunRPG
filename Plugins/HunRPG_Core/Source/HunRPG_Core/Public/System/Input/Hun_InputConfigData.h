// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hun_InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_CORE_API UHun_InputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Dash;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Jump;
};
