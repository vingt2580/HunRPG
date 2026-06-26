// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_HunHPBar.h"
#include "Widget_HunMonsterHPBar.generated.h"

class AHun_MonsterBase;
/**
 * 
 */
UCLASS()
class HUNRPG_API UWidget_HunMonsterHPBar : public UWidget_HunHPBar
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
};
