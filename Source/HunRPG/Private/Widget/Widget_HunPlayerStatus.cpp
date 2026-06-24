// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_HunPlayerStatus.h"

#include "Components/Hun_CombatComponent.h"
#include "Components/ProgressBar.h"

void UWidget_HunPlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_HunPlayerStatus::UpdateHPBar(float CurrentHP, float MaxHP)
{
	if (HPProgressBar && MaxHP > 0.0f)
	{
		float HPPercentage = CurrentHP / MaxHP;
		HPProgressBar->SetPercent(HPPercentage);
	}
}

void UWidget_HunPlayerStatus::BindCombatComponent(UHun_CombatComponent* CombatComponent)
{
	if (CombatComponent)
	{
		CombatComponent->OnHPChange.AddDynamic(this, &UWidget_HunPlayerStatus::UpdateHPBar);
		UpdateHPBar(CombatComponent->CurrentHealthPoint, CombatComponent->MaxHealthPoint);
	}
}
