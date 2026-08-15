// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_StampedKnockUp.h"

#include "Character/Hun_Grux.h"

void UHun_AnimNotify_StampedKnockUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AHun_Grux* Grux = Cast<AHun_Grux>(MeshComp->GetOwner());
		if (IsValid(Grux))
		{
			Grux->ExecuteStampedKnockup();
		}
	}
}
