// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_ChainExplosion.h"

#include "HunRPG_DebugHelper.h"
#include "Character/Hun_Grux.h"

class AHun_Grux;

void UHun_AnimNotify_ChainExplosion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		AHun_Grux* Grux = Cast<AHun_Grux>(MeshComp->GetOwner());
		if (IsValid(Grux))
		{
			Grux->ExecuteChainExplosion();

			HUN_LOG(FColor::Red, "체인익스플로젼");
		}
	}
}
