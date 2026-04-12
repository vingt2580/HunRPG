// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hun_MovementInterface.generated.h"

UINTERFACE()
class UHun_MovementInterface : public UInterface
{
	GENERATED_BODY()
};

class HUNRPG_CORE_API IHun_MovementInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void SetMoveSpeed_Interface(float MoveSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void MovementInput_Interface(FVector2d MoveVector);
	
	/**
	 * Implementation
	 */
	
	virtual void MovementInput_Implementation(FVector2d MoveVector);

	virtual  void SetMoveSpeed_Implementation(float MoveSpeed);
};
