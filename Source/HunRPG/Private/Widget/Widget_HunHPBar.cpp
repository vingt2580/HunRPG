// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_HunHPBar.h"

#include "Components/Hun_CombatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWidget_HunHPBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_HunHPBar::UpdateHPBar(float CurrentHP, float MaxHP)
{
	if (HPProgressBar && MaxHP > 0.0f)
	{
		float HPPercentage = CurrentHP / MaxHP;
		HPProgressBar->SetPercent(HPPercentage);
	}

	if (CurrentHPTextBlock && MaxHPTextBlock)
	{
		FText CurrentHPText = FText::AsNumber(CurrentHP);
		FText MaxHPText = FText::AsNumber(MaxHP);
		
		CurrentHPTextBlock->SetText(CurrentHPText);
		MaxHPTextBlock->SetText(MaxHPText);
	}
}

void UWidget_HunHPBar::BindCombatComponent(UHun_CombatComponent* CombatComponent)
{
	if (CombatComponent)
	{
		CombatComponent->OnHPChange.AddDynamic(this, &UWidget_HunHPBar::UpdateHPBar);
		UpdateHPBar(CombatComponent->CurrentHealthPoint, CombatComponent->MaxHealthPoint);
	}
}
