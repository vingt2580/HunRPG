// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Hun_PlayBossPattern_BTTask.h"

#include "AIController.h"
#include "Components/Hun_CombatComponent.h"

UHun_PlayBossPattern_BTTask::UHun_PlayBossPattern_BTTask()
{
	NodeName =TEXT("Play Boss Pattern");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UHun_PlayBossPattern_BTTask::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	CachedOwnerComp = &Comp;
	AAIController* AIController = Comp.GetAIOwner();

	if (IsValid(AIController) && IsValid(BossPatternMontage))
	{
		APawn* BossPawn = AIController->GetPawn();
		if (IsValid(BossPawn))
		{
			UHun_CombatComponent* CombatComp = BossPawn->FindComponentByClass<UHun_CombatComponent>();
			if (IsValid(CombatComp))
			{
				CombatComp->OnPatternFinished.AddUniqueDynamic(this, &UHun_PlayBossPattern_BTTask::OnPatternFinished);
				CombatComp->ExecutePattern(BossPatternMontage);

				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UHun_PlayBossPattern_BTTask::OnPatternFinished()
{
	if (IsValid(CachedOwnerComp))
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
