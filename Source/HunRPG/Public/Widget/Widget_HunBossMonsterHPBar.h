// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_HunHPBar.h"
#include "Widget_HunBossMonsterHPBar.generated.h"

/**
 * 
 */
UCLASS()
class HUNRPG_API UWidget_HunBossMonsterHPBar : public UWidget_HunHPBar
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossMainNameText;
};
