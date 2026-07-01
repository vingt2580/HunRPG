// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_ActorComponent.h"
#include "Components/ActorComponent.h"
#include "HunRPG/Public/Components/Hun_StateComponent.h"
#include "HunRPG/Public/Interface/Hun_MovementInterface.h"
#include "Hun_MoveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FonStaminaDelegate, float, CurrentStatmina, float, MaxStamina);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUNRPG_API UHun_MoveComponent : public UHun_ActorComponent, public IHun_MovementInterface
{
	GENERATED_BODY()

	UHun_MoveComponent();

	virtual void MovementInput_Interface_Implementation(FVector2D MoveVector) override;

	virtual void ApplyStateSpeed_Interface_Implementation(EHunRPG_ActionState State) override;

	virtual void JumpInput_interface_Implementation() override;

	virtual void DashInput_Interface_Implementation() override;

	virtual void SetMoveSpeed_Interface_Implementation(float Speed) override;
	
	virtual void InitializeMobData() override;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CanJump();
	bool CanMove();

public:
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MoveComponent;
	UPROPERTY()
	TObjectPtr<UHun_StateComponent> StateComponent;

	UPROPERTY()
	float MaxStamina;
	UPROPERTY()
	float CurrentStamina;
	UPROPERTY()
	float StaminaReganFigure;
	UPROPERTY()
	float StaminaReganDelay;

	float TimeSinceLastStaminaUse;
	bool bIsRunning;

	bool ConsumeStamina(float Value);

	void SetRunningState(bool bRunning);

	UPROPERTY(BlueprintAssignable)
	FonStaminaDelegate OnStaminaUpdate;
};
