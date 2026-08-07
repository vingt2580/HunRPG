// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Hun_ChangeBossPhase.h"

#include "AIController.h"
#include "Character/Hun_BossMonsterBase.h"

UHun_ChangeBossPhase::UHun_ChangeBossPhase()
{
	NodeName = TEXT("Change BossPhase");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UHun_ChangeBossPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!bChangedBossPhase)
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		
		if (IsValid(AIController))
		{
			APawn* OwnerPawn = AIController->GetPawn();

			if (IsValid(OwnerPawn))
			{
				AHun_BossMonsterBase* OwnerBossMonster = Cast<AHun_BossMonsterBase>(OwnerPawn);

				if (IsValid(OwnerBossMonster))
				{
					OwnerBossMonster->ExecuteChangePhase(BossPhase);
					bChangedBossPhase = true;
		
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
