// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Hun_AnimNotify_StartRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_AnimNotify_StartRangeAttack : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
