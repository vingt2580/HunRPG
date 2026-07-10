// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HunHPBar.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;
class UHun_CombatComponent;

UCLASS()
class HUNRPG_API UWidget_HunHPBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHPTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHPTextBlock;

public:
	UFUNCTION()
	void UpdateHPBar(float CurrentHP, float MaxHP);

	void BindCombatComponent(UHun_CombatComponent* CombatComponent);
	void UnbindCombatComponent(UHun_CombatComponent* CombatComponent);
};
