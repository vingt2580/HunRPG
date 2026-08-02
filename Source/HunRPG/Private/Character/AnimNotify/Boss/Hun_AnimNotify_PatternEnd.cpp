// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_PatternEnd.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Hun_MobBase.h"

void UHun_AnimNotify_PatternEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (MeshComp && Owner)
	{
		AHun_MobBase* OwnerMob = Cast<AHun_MobBase>(Owner);
		AAIController* AIController = Cast<AAIController>(OwnerMob->GetController());

		if (IsValid(AIController))
		{
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
			
			if (IsValid(BlackboardComp))
			{
				BlackboardComp->SetValueAsBool(FName(FName("IsPatterning")), false);
			}
		}
	}
}
