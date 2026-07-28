// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Widget_HunPartyMemberList.h"

#include "Character/Hun_Character.h"
#include "HunRPG/Public/Components/Hun_CombatComponent.h"
#include "Components/Image.h"
#include "Controller/PlayerController/Hun_PlayerController.h"
#include "Data/Hun_CharacterData.h"
#include "Widget/Widget_HunHPBar.h"
#include "Widget/Widget_HunPartyMemberProfile.h"

void UWidget_HunPartyMemberList::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateMembers();
}

void UWidget_HunPartyMemberList::UpdateMembers() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController))
		return;
	
	AHun_PlayerController* PC = Cast<AHun_PlayerController>(PlayerController);

	if (IsValid(PC->HunCharacterPartyMembers[0]))
	{
		if (UTexture2D* MemberTexture_1 = PC->HunCharacterPartyMembers[0]->CharacterData->ProfileImage)
		{
			HunPartyMember_1->CharacterImage->SetBrushFromTexture(MemberTexture_1);
			HunPartyMember_1->SetVisibility(ESlateVisibility::Visible);
		}

		HunPartyMember_1->CharacterHPBar->BindCombatComponent(PC->HunCharacterPartyMembers[0]->GetComponentByClass<UHun_CombatComponent>());
	}	
	if (IsValid(PC->HunCharacterPartyMembers[1]))
	{
		if (UTexture2D* MemberTexture_2 = PC->HunCharacterPartyMembers[1]->CharacterData->ProfileImage)
		{
			HunPartyMember_2->CharacterImage->SetBrushFromTexture(MemberTexture_2);
			HunPartyMember_2->SetVisibility(ESlateVisibility::Visible);	
		}
		HunPartyMember_2->CharacterHPBar->BindCombatComponent(PC->HunCharacterPartyMembers[1]->GetComponentByClass<UHun_CombatComponent>());
	}
	if (IsValid(PC->HunCharacterPartyMembers[2]))
	{
		if (UTexture2D* MemberTexture_3 = PC->HunCharacterPartyMembers[2]->CharacterData->ProfileImage)
		{
			HunPartyMember_3->CharacterImage->SetBrushFromTexture(MemberTexture_3);
			HunPartyMember_3->SetVisibility(ESlateVisibility::Visible);	
		}
		HunPartyMember_3->CharacterHPBar->BindCombatComponent(PC->HunCharacterPartyMembers[2]->GetComponentByClass<UHun_CombatComponent>());
	}
}
