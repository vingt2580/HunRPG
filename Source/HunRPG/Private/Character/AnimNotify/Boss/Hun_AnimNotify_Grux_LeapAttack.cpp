// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_Grux_LeapAttack.h"

#include "HunRPG_DebugHelper.h"
#include "Character/Hun_Grux.h"

void UHun_AnimNotify_Grux_LeapAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		AHun_Grux* Grux = Cast<AHun_Grux>(MeshComp->GetOwner());
		if (IsValid(Grux))
		{
			if (Grux->bIsLeaping)
				return;
			
			Grux->ExecuteLeapAttack_BossPattern();

			HUN_LOG(FColor::Red, "점프 공격 시작!!");
		}
	}
}
