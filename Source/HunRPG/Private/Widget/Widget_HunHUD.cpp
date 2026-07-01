// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_HunHUD.h"

#include "Components/Hun_CombatComponent.h"
#include "Components/Hun_MoveComponent.h"
#include "Widget/Widget_HunPlayerHPBar.h"
#include "Widget/WIdget_HunStaminaGauge.h"

void UWidget_HunHUD::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();

	UHun_CombatComponent* CombatComp = PC->GetPawn()->FindComponentByClass<UHun_CombatComponent>();
	UHun_MoveComponent* MoveComp = PC->GetPawn()->FindComponentByClass<UHun_MoveComponent>();

	if (CombatComp)
	{
		PlayerStatusWidget->BindCombatComponent(CombatComp);
	}
	if (MoveComp)
	{
		StaminaGauge->BindMoveComponent(MoveComp);
	}
}
