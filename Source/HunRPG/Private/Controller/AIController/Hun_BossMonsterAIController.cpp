// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/Hun_BossMonsterAIController.h"

#include "HunRPG_DebugHelper.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void AHun_BossMonsterAIController::StartBossCombat(AActor* TargetPlayer)
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	
	if (!IsValid(BBComp) || !IsValid(TargetPlayer))
		return;

	BBComp->SetValueAsObject(TEXT("TargetPlayer"), TargetPlayer);
	BBComp->SetValueAsBool(TEXT("IsCombat"),true);

	bIsGetTarget = true;

	HUN_LOG(FColor::Green, "보스 AI: 타겟확인 전투시작");
}

void AHun_BossMonsterAIController::ChangeBossPhase(int32 NewPhase)
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	if (!IsValid(BBComp))
		return;

	BBComp->SetValueAsInt(TEXT("Phase"), NewPhase);
}

void AHun_BossMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(BossBehaviorTree))
		return;

	RunBehaviorTree(BossBehaviorTree);

	if (UBlackboardComponent* BBComp = GetBlackboardComponent())
	{
		BBComp->SetValueAsInt(TEXT("Phase"), 1);
		BBComp->SetValueAsBool(TEXT("IsCombat"),false);
	}
}
