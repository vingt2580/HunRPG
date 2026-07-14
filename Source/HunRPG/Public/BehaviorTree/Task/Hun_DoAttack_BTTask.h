// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Hun_CheckingDisrtance_BTService.h"
#include "Hun_DoAttack_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_DoAttack_BTTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UHun_DoAttack_BTTask();

protected:
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
