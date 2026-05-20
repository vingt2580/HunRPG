// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Hun_MobData.h"
#include "GameFramework/Character.h"
#include "Hun_MobBase.generated.h"

UCLASS()
class HUNRPG_GAMEPLAY_API AHun_MobBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHun_MobBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die(AActor* DamageCauser);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|MobData")
	UHun_MobData* MobData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Combat")
	float HitAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Stat")
	int CurrentHealthPoint;

	void CheckHitAngle(AActor* DamageCauser);
	void PlayHitAnimation();
	void PlayDeathAnimation();
	

	UPROPERTY(BlueprintReadWrite, Category = "HunRPG|Combat")
	bool IsHit;
	UPROPERTY(BlueprintReadWrite, Category = "HunRPG|Combat")
	bool IsDeath = false;
};
