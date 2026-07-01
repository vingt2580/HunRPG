// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Hun_AnimNotify_EndAbility.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_AnimNotify_EndAbility : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
