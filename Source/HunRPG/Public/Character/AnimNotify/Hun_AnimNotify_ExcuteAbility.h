// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Hun_AnimNotify_ExcuteAbility.generated.h"

class UHun_AbilityData;
/**
 * 
 */
UCLASS()
class HUNRPG_API UHun_AnimNotify_ExcuteAbility : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY()
	const UHun_AbilityData* AbilityData;

	UPROPERTY()
	FName AbilitySocket = NAME_None;

	
};
