// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HunRPG_Core/Public/Interface/Hun_CombatInterface.h"
#include "Hun_CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUN_COMPONENTS_API UHun_CombatComponent : public UActorComponent, public IHun_CombatInterface
{
	GENERATED_BODY()

	virtual void AttackInput_interface_Implementation() override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
