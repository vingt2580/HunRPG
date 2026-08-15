// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_StampedStart.h"

#include "HunRPG_DebugHelper.h"
#include "Character/Hun_Grux.h"

void UHun_AnimNotify_StampedStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AHun_Grux* Grux = Cast<AHun_Grux>(MeshComp->GetOwner());
		if (IsValid(Grux))
		{
			Grux->ExecuteStampedStart();

			HUN_LOG(FColor::Red, "스탬피드");
		}
	}
}
