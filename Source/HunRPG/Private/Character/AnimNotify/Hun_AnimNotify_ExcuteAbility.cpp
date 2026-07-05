// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/Hun_AnimNotify_ExcuteAbility.h"

#include "Character/Hun_MobBase.h"
#include "Components/Hun_CombatComponent.h"
#include "Data/Hun_AbilityData.h"

void UHun_AnimNotify_ExcuteAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
		return;

	UWorld* World = MeshComp->GetWorld();
	if (IsValid(World) == false || World->IsGameWorld() == false)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	AHun_MobBase* Hun_Mob = Cast<AHun_MobBase>(Owner);

	if (!IsValid(Hun_Mob))
		return;

	UHun_CombatComponent* CombatComp = Hun_Mob->FindComponentByClass<UHun_CombatComponent>();

	FVector ActivePoint;
	FRotator ActiveRotator;

	if (AbilitySocket != NAME_None && MeshComp->DoesSocketExist(AbilitySocket))
	{
		ActivePoint = MeshComp->GetSocketLocation(AbilitySocket);
		ActiveRotator = MeshComp->GetSocketRotation(AbilitySocket);
	}
	else
	{
		ActivePoint = Hun_Mob->GetActorLocation();
		ActiveRotator = Hun_Mob->GetActorRotation();
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	FName CurrentSection = AnimInstance->Montage_GetCurrentSection();

	if (AnimInstance)
	{
		if (CurrentSection == NAME_None)
		{
			return;
		}
		else if (CurrentSection == TEXT("Ability_A"))
		{
			AbilityData = CombatComp->GetAbility_A_Data();
		}
		else if (CurrentSection == TEXT("Ability_B"))
		{
			AbilityData = CombatComp->GetAbility_B_Data();
		}
		else if (CurrentSection == TEXT("Ultimate"))
		{
			AbilityData = CombatComp->GetUltimate_Data();
		}
	}
	
	CombatComp->ExecuteAbility(AbilityData->AbilityInfo, ActivePoint, ActiveRotator);
}
