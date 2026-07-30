// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/Hun_Character.h"
#include "Hun_BossMonsterAIController.generated.h"

UCLASS()
class HUNRPG_API AHun_BossMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHun_BossMonsterAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|AI")
	class UBehaviorTree* BossBehaviorTree;

	UFUNCTION(BlueprintCallable, Category = "HunRPG|AI")
	void StartBossCombat(AActor* TargetPlayer);

	UFUNCTION(BlueprintCallable, Category = "HunRPG|AI")
	void ChangeBossPhase(int32 NewPhase);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
