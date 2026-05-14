// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_MobBase.h"
#include "Hun_MonsterBase.generated.h"

UCLASS()
class HUNRPG_GAMEPLAY_API AHun_MonsterBase : public AHun_MobBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHun_MonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
