// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WIdget_HunStaminaGauge.generated.h"

/**
 * 
 */
class UImage;

UCLASS()
class HUNRPG_API UWIdget_HunStaminaGauge : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* RoundGauge;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | Stamina")
	float StaminaPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Material")
	class UMaterialInstance* RoundGaugeMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG | Material")
	class UMaterialInstanceDynamic* DynamicRoundGaugeMaterial;

private:
	void UpdateRoundGauge(float GaugeValue);
	void SetStaminaGauge();
	
};
