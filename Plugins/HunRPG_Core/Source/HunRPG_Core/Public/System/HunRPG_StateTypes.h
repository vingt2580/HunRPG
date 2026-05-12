// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HunRPG_StateTypes.generated.h"

UENUM(BlueprintType)
enum class EHunRPG_ActionState : uint8
{
	Idle,
	Moving,
	Dashing,
	Running,
	Jumping,
	Falling,
	Attacking,
	HitReaction,
	Dead
};

USTRUCT(BlueprintType)
struct FHun_ActionValue
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WalkSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RunSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DashLength = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LookSensitivity = 0.5;
};

USTRUCT(BlueprintType)
struct FHun_CombatValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* ComboMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxComboCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 10.0f;
};
