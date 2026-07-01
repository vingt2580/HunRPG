// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WIdget_HunStaminaGauge.generated.h"

/**
 * 
 */
class UHun_MoveComponent;
class UImage;

UCLASS()
class HUNRPG_API UWIdget_HunStaminaGauge : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* RoundGauge;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HunRPG | Stamina")
	float StaminaPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Material")
	class UMaterialInstance* RoundGaugeMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG | Material")
	class UMaterialInstanceDynamic* DynamicRoundGaugeMaterial;

	UFUNCTION(BlueprintCallable, Category = "HunRPG | Stamina")
	void UpdateRoundGauge(float GaugeValue);

	UFUNCTION()
	void OnStaminaUpdated(float CurrentStamina, float MaxStamina);

	void BindMoveComponent(UHun_MoveComponent* MovementComponent);

private:
	void SetStaminaGauge();

	UPROPERTY()
	class AActor* TargetCharacter;
	
};
