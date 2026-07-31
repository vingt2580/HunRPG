// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Hun_PlayBossPattern_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_PlayBossPattern_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UHun_PlayBossPattern_BTTask();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Pattern")
	UAnimMontage* BossPatternMontage;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnPatternFinished();

private:
	UBehaviorTreeComponent* CachedOwnerComp;
};
