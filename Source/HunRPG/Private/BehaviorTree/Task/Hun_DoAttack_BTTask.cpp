// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Hun_DoAttack_BTTask.h"

#include "AIController.h"
#include "HunRPG_DebugHelper.h"
#include "Components/Hun_CombatComponent.h"
#include "Components/Hun_MoveComponent.h"
#include "Interface/Hun_CombatInterface.h"

UHun_DoAttack_BTTask::UHun_DoAttack_BTTask()
{
	NodeName = TEXT("DoAttack");
}

EBTNodeResult::Type UHun_DoAttack_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (!AIController || !AIController->GetPawn())
		return EBTNodeResult::Failed;


	AHun_MobBase* Mob = Cast<AHun_MobBase>(AIController->GetPawn());
	
	UHun_CombatComponent* CombatComponent = Mob->GetComponentByClass<UHun_CombatComponent>();
	CombatComponent->StartComboAttack();

	HUN_LOG(FColor::Red,"AI 공격 시작");
	
	return EBTNodeResult::Succeeded;
}
