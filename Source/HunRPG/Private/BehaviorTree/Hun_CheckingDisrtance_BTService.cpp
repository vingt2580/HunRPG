// Fill out your copyright notice in the Description page of Project Settings.


#include "HunRPG/Public/BehaviorTree/Hun_CheckingDisrtance_BTService.h"

#include "HunRPG_DebugHelper.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Hun_BossMonsterBase.h"
#include "Components/Hun_CombatComponent.h"
#include "Data/Hun_CharacterData.h"

UHun_CheckingDisrtance_BTService::UHun_CheckingDisrtance_BTService()
{
	NodeName = TEXT("Check Distance to Target");
	Interval = 0.1f;
}

void UHun_CheckingDisrtance_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!BlackboardComponent || !AIController || !AIController->GetPawn())
		return;

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!TargetActor)
	{
		BlackboardComponent->SetValueAsBool(IsFoundTarget.SelectedKeyName, false);
		return;
	}

	if (TargetActor)
	{
		float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());

		if (Distance <= 100.0f)
		{
			//HUN_LOG(FColor::Red,"Distance is %f",Distance);
		}
		BlackboardComponent->SetValueAsBool(IsFoundTarget.SelectedKeyName, true);
		BlackboardComponent->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);

		AHun_MobBase* OwnerMob = Cast<AHun_MobBase>(AIController->GetPawn());
		if (!IsValid(OwnerMob))
			return;

		UHun_CombatComponent* CombatComp = OwnerMob->FindComponentByClass<UHun_CombatComponent>();
		
		float CurrentHP = CombatComp->CurrentHealthPoint;
		float MaxHP = CombatComp->GetMobData()->MaxHealthPoint;

		float OwnerHPPercent = (MaxHP > 0.f) ? (CurrentHP/MaxHP) * 100.f : 0.f;
		
		BlackboardComponent->SetValueAsFloat(BossHPPercent.SelectedKeyName, OwnerHPPercent);
	}
}
