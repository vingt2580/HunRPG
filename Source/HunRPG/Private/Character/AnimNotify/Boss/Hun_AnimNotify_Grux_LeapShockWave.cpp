// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Boss/Hun_AnimNotify_Grux_LeapShockWave.h"

#include "HunRPG_DebugHelper.h"
#include "Character/Hun_Grux.h"

void UHun_AnimNotify_Grux_LeapShockWave::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		AHun_Grux* Grux = Cast<AHun_Grux>(MeshComp->GetOwner());
		if (IsValid(Grux))
		{
			Grux->ExecuteLAndingShockwave();

			HUN_LOG(FColor::Red, "쇼크웨이브");
		}
	}
}
