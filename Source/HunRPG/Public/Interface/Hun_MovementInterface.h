// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_Interface.h"
#include "System/HunRPG_StateTypes.h"
#include "UObject/Interface.h"
#include "Hun_MovementInterface.generated.h"

UINTERFACE()
class UHun_MovementInterface : public UHun_Interface
{
	GENERATED_BODY()
};

class HUNRPG_API IHun_MovementInterface : public IHun_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void SetMoveSpeed_Interface(FHun_ActionValue MoveSpeed, EHunRPG_ActionState State);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void MovementInput_Interface(FVector2D MoveVector);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void JumpInput_interface();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void DashInput_Interface();
	
	/**
	 * Implementation
	 */
	
	virtual void MovementInput_Interface_Implementation(FVector2D MoveVector);

	virtual  void SetMoveSpeed_Interface_Implementation(FHun_ActionValue MoveSpeed, EHunRPG_ActionState State);

	virtual  void JumpInput_Interface_Implementation();

	virtual  void DashInput_Interface_Implementation();
};
