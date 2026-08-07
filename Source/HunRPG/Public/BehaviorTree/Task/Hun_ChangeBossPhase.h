// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Hun_ChangeBossPhase.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_ChangeBossPhase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UHun_ChangeBossPhase();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BossPhase")
	int32 BossPhase;

private:
	bool bChangedBossPhase = false;
};
