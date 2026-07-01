// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Hun_AnimNotify_EndAbility.h"

#include "Components/Hun_StateComponent.h"

void UHun_AnimNotify_EndAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (UHun_StateComponent* StateComp = MeshComp->GetOwner()->FindComponentByClass<UHun_StateComponent>())
		{
			StateComp->SetState(EHunRPG_ActionState::Idle);
		}
	}
}
