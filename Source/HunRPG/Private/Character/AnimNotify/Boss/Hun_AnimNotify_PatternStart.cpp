// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_PatternStart.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Hun_MobBase.h"
#include "Components/CapsuleComponent.h"

void UHun_AnimNotify_PatternStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	
	if (MeshComp && Owner)
	{
		AHun_MobBase* OwnerMob = Cast<AHun_MobBase>(Owner);
		if (IsValid(OwnerMob))
		{
			AAIController* AIController = Cast<AAIController>(OwnerMob->GetController());
			
			if (!IsValid(AIController)) return;
			
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
			
			if (IsValid(BlackboardComp))
			{
				UCapsuleComponent* MobCollision = OwnerMob->GetCapsuleComponent();
				MobCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				BlackboardComp->SetValueAsBool(FName(FName("IsPatterning")), true);
			}
		}
	}
}
