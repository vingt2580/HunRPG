// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HunRPG_Core/Public/System/HunRPG_StateTypes.h"
#include "Hun_StateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUN_COMPONENTS_API UHun_StateComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	EHunRPG_ActionState CurrentState;

public:
	void SetState(EHunRPG_ActionState NewState) { CurrentState = NewState; };

	EHunRPG_ActionState GetState() const { return CurrentState; };

	bool IsState(EHunRPG_ActionState CheckState) const { return CurrentState == CheckState; };
};
