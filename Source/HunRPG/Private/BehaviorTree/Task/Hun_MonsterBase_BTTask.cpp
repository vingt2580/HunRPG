// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Hun_MonsterBase_BTTask.h"

#include "Character/Hun_MonsterBase.h"

EBTNodeResult::Type UHun_MonsterBase_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHun_MonsterBase* OwnerMonster = Cast<AHun_MonsterBase>(OwnerComp.GetOwner());

	if (!OwnerMonster)
		return EBTNodeResult::Failed;
	
	return EBTNodeResult::Succeeded;
}
