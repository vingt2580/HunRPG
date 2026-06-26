// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_HunHUD.h"

#include "Components/Hun_CombatComponent.h"
#include "Widget/Widget_HunPlayerHPBar.h"

void UWidget_HunHUD::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();

	UHun_CombatComponent* CombatComp = PC->GetPawn()->FindComponentByClass<UHun_CombatComponent>();

	if (CombatComp)
	{
		PlayerStatusWidget->BindCombatComponent(CombatComp);
	}
}
