// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hun_MobBase.generated.h"

class UHun_AbilityData;
class UHun_CharacterData;
class UHun_ComponentsData;
class UHun_ActorComponent;
UCLASS()
class HUNRPG_API AHun_MobBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHun_MobBase();

	void SetMobSpeed(float speed);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HunRPG | Components")
	TObjectPtr<UHun_ComponentsData> ComponentsData;
	UPROPERTY()
	TObjectPtr<UHun_ActorComponent> CachedMovementComponent;
	UPROPERTY()
	TObjectPtr<UHun_ActorComponent> CachedCombatComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | CharacterData")
	UHun_CharacterData* CharacterData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | AbilityData")
	UHun_AbilityData* Ability_A_Data;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | AbilityData")
	UHun_AbilityData* Ability_B_Data;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | AbilityData")
	UHun_AbilityData* Ultimate_Data;

	UPROPERTY()
	TArray<UHun_ActorComponent*> Component;
};
