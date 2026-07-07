// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hun_InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_InputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Dash;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Ablity_A;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Ablity_B;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputAction* Input_Ultimate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HunRPG|SwapCharacter")
	class UInputAction* Input_Swap1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HunRPG|SwapCharacter")
	class UInputAction* Input_Swap2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HunRPG|SwapCharacter")
	class UInputAction* Input_Swap3;
};
