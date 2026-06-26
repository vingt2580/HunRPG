// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HunHUD.generated.h"

/**
 * 
 */
class UWidget_HunPlayerHPBar;

UCLASS()
class HUNRPG_API UWidget_HunHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta=(BindWidget))
	UWidget_HunPlayerHPBar* PlayerStatusWidget;
};
