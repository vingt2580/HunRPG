// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HunPartyMemberProfile.generated.h"

class UWidget_HunHPBar;
class UImage;
/**
 * 
 */
UCLASS()
class HUNRPG_API UWidget_HunPartyMemberProfile : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage;
	UPROPERTY(meta = (BindWidget))
	UWidget_HunHPBar* CharacterHPBar;
};
