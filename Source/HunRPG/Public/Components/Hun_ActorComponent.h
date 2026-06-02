// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Hun_MobBase.h"
#include "Components/ActorComponent.h"
#include "Hun_ActorComponent.generated.h"

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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | Data")
	TObjectPtr<UHun_CharacterData> MobData;

	UFUNCTION()
	AHun_MobBase* GetOwnerCharacter() const { return OwnerCharacter; }
	UFUNCTION()
	UHun_CharacterData* GetMobData() const { return MobData; }
	void SetData(UHun_CharacterData* NewHunData);

protected:
	virtual void BeginPlay() override;

	virtual void InitializeMobData();
};
