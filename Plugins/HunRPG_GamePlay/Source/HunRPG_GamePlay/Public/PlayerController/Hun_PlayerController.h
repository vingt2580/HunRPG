// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"

#include "HunRPG_Core/Public/System/Input/Hun_InputConfigData.h"
#include "HunRPG_GamePlay/Public/Character/Hun_Character.h"
#include "Hun_PlayerController.generated.h"

/**
 * 
 */

class FinputActionValue;

UCLASS()
class HUNRPG_GAMEPLAY_API AHun_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|InputData")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Controller")
	class UHun_InputConfigData* InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Character")
	class AHun_Character* HunCharacter;
	
public:
	void Input_Move(const FInputActionValue& ActionValue);
	void Input_Jump();
};
