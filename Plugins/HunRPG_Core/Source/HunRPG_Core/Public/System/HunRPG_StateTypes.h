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
