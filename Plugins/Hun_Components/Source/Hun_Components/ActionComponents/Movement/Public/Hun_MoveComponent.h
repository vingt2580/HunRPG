// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HunRPG_Core/Public/Interface/Hun_MovementInterface.h"
#include "Hun_MoveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUN_COMPONENTS_API UHun_MoveComponent : public UActorComponent, public IHun_MovementInterface
{
	GENERATED_BODY()

	virtual void MovementInput_Interface_Implementation(FVector2D MoveVector) override;

	virtual void SetMoveSpeed_Interface_Implementation(float MoveSpeed) override;

	virtual  void JumpInput_interface_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCharacterMovementComponent* MoveComponent;
	UPROPERTY()
	class UHun_StateComponent* StateComponent;
};
