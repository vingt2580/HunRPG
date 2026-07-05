// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HunRPG/Public/Character/Hun_MobBase.h"
#include "Hun_ActorComponent.generated.h"

class UHun_AbilityData;
class UHun_CharacterData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUNRPG_API UHun_ActorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UHun_ActorComponent();

	UPROPERTY()
	TObjectPtr<AHun_MobBase> OwnerCharacter;

	UFUNCTION()
	AHun_MobBase* GetOwnerCharacter() const { return OwnerCharacter; }
	UFUNCTION()
	UHun_CharacterData* GetMobData() const { return OwnerCharacter->CharacterData; }
	UFUNCTION()
	UHun_AbilityData* GetAbility_A_Data() const { return  OwnerCharacter->Ability_A_Data; }
	UFUNCTION()
	UHun_AbilityData* GetAbility_B_Data() const { return  OwnerCharacter->Ability_B_Data; }
	UFUNCTION()
	UHun_AbilityData* GetUltimate_Data() const { return  OwnerCharacter->Ultimate_Data; }
	void SetData(UHun_CharacterData* NewHunData);

protected:
	virtual void BeginPlay() override;

	virtual void InitializeMobData();
};
