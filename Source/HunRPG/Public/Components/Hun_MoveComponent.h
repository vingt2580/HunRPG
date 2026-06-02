// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_ActorComponent.h"
#include "Components/ActorComponent.h"
#include "HunRPG/Public/Interface/Hun_MovementInterface.h"
#include "Hun_MoveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUNRPG_API UHun_MoveComponent : public UHun_ActorComponent, public IHun_MovementInterface
{
	GENERATED_BODY()

	virtual void MovementInput_Interface_Implementation(FVector2D MoveVector) override;

	virtual void SetMoveSpeed_Interface_Implementation(EHunRPG_ActionState State) override;

	virtual void JumpInput_interface_Implementation() override;

	virtual void DashInput_Interface_Implementation() override;

	virtual void InitializeMobData() override;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CanJump();
	bool CanMove();
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MoveComponent;
	UPROPERTY()
	TObjectPtr<UHun_StateComponent> StateComponent;
};
