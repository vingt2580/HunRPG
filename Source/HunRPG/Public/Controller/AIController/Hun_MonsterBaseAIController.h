// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/Hun_Character.h"
#include "Hun_MonsterBaseAIController.generated.h"

UCLASS()
class HUNRPG_API AHun_MonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	AHun_MonsterBaseAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SwapTargetPlayer(AHun_Character* TargetPlayer);

	bool bIsGetTarget = false; 
};
