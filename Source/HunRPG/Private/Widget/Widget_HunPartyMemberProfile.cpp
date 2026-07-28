// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_HunPartyMemberProfile.h"

#include "Components/TextBlock.h"
#include "Controller/PlayerController/Hun_PlayerController.h"
#include "Widget/Widget_HunHPBar.h"

void UWidget_HunPartyMemberProfile::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
	CharacterHPBar->MaxHPTextBlock->SetVisibility(ESlateVisibility::Hidden);
	CharacterHPBar->CurrentHPTextBlock->SetVisibility(ESlateVisibility::Hidden);
}
