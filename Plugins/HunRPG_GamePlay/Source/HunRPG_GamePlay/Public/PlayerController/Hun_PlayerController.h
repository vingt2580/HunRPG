// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"

#include "HunRPG_Core/Public/System/Input/Hun_InputConfigData.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HunRPG|InputData")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HunRPG | Controller")
	class UHun_InputConfigData* InputActions;

public:
	void Input_Move(const FInputActionValue& ActionValue);
};
