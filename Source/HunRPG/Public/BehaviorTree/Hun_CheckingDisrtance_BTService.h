// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Hun_CheckingDisrtance_BTService.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_CheckingDisrtance_BTService : public UBTService
{
	GENERATED_BODY()

public:
	UHun_CheckingDisrtance_BTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "HunRPG|AI_BT")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "HunRPG|AI_BT")
	FBlackboardKeySelector DistanceKey;

	UPROPERTY(EditAnywhere, Category = "HunRPG|AI_BT")
	FBlackboardKeySelector IsFoundTarget;
	
	UPROPERTY(EditAnywhere, Category = "HunRPG|AI_BT")
	FBlackboardKeySelector IsSkillReady;

	UPROPERTY(EditAnywhere, Category = "HunRPG|AI_BT")
	FBlackboardKeySelector SkillCooldown;
};
