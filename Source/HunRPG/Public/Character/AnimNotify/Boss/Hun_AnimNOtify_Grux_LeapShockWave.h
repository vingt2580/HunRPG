// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Hun_AnimNOtify_Grux_LeapShockWave.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_AnimNOtify_Grux_LeapShockWave : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
