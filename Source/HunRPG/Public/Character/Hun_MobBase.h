// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hun_CombatInterface.h"
#include "Hun_MobBase.generated.h"

class UHun_CharacterData;
UCLASS()
class HUNRPG_API AHun_MobBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHun_MobBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Combat")
	float HitAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Stat")
	int CurrentHealthPoint;
	
	void PlayDeathAnimation();

	TScriptInterface<IHun_CombatInterface> CombatInterface = this;

	UPROPERTY(BlueprintReadWrite, Category = "HunRPG|Combat")
	bool IsHit;
	UPROPERTY(BlueprintReadWrite, Category = "HunRPG|Combat")
	bool IsDeath = false;
};
