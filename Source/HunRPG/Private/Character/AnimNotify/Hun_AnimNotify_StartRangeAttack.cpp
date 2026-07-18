// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Hun_AnimNotify_StartRangeAttack.h"

#include "Components/Hun_CombatComponent.h"

void UHun_AnimNotify_StartRangeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (UHun_CombatComponent* CombatComp = MeshComp->GetOwner()->FindComponentByClass<UHun_CombatComponent>())
		{
			CombatComp->RangeAttack();
		}
	}
}
