// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Hun_AnimNotify_ResetCombo.h"
#include "HunRPG/Public/Components/Hun_CombatComponent.h"

void UHun_AnimNotify_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (UHun_CombatComponent* CombatComp = MeshComp->GetOwner()->FindComponentByClass<UHun_CombatComponent>())
		{
			CombatComp->OnResetCombo();
		}
	}
}
