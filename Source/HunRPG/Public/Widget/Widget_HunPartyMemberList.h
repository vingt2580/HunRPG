// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HunPartyMemberList.generated.h"

class UWidget_HunPartyMemberProfile;
/**
 * 
 */
UCLASS()
class HUNRPG_API UWidget_HunPartyMemberList : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UWidget_HunPartyMemberProfile* HunPartyMember_1;
	UPROPERTY(meta = (BindWidget))
	UWidget_HunPartyMemberProfile* HunPartyMember_2;
	UPROPERTY(meta = (BindWidget))
	UWidget_HunPartyMemberProfile* HunPartyMember_3;

	void UpdateMembers() const;
};
