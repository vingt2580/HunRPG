// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_MobBase.h"
#include "Components/WidgetComponent.h"
#include "Hun_MonsterBase.generated.h"

UCLASS()
class HUNRPG_API AHun_MonsterBase : public AHun_MobBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	AHun_MonsterBase();

	UFUNCTION()
	void BindStatusBar();

	void OrientHpBar();

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="HunRPG | Widget")
	TObjectPtr<UWidgetComponent> Widget_MonsterHPBar;
};
