// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WIdget_HunStaminaGauge.h"

#include "Components/Image.h"

void UWIdget_HunStaminaGauge::NativeConstruct()
{
	Super::NativeConstruct();

	DynamicRoundGaugeMaterial = UMaterialInstanceDynamic::Create(RoundGaugeMaterial, this);

	SetStaminaGauge();
}

void UWIdget_HunStaminaGauge::UpdateRoundGauge(float GaugeValue)
{
	if (IsValid(DynamicRoundGaugeMaterial))
	{
		DynamicRoundGaugeMaterial->SetScalarParameterValue(FName("Percent"), GaugeValue);
	}
}

void UWIdget_HunStaminaGauge::SetStaminaGauge()
{
	if (IsValid(DynamicRoundGaugeMaterial))
	{
		FSlateBrush RoundGaugeBrush;
		RoundGaugeBrush.SetResourceObject(DynamicRoundGaugeMaterial);
		
		RoundGauge->SetBrush(RoundGaugeBrush);
		
		DynamicRoundGaugeMaterial->SetScalarParameterValue(FName("Percent"), 1);
	}
}
