// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WIdget_HunStaminaGauge.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Hun_MoveComponent.h"
#include "Components/Image.h"

void UWIdget_HunStaminaGauge::NativeConstruct()
{
	Super::NativeConstruct();

	DynamicRoundGaugeMaterial = UMaterialInstanceDynamic::Create(RoundGaugeMaterial, this);

	SetStaminaGauge();
}

void UWIdget_HunStaminaGauge::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TargetCharacter)
		return;

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FVector WorldLocation = TargetCharacter->GetActorLocation() + FVector(0, 50.f, 50.f);
		FVector2D ScreenPosition;

		if (bool bIsOnScreen = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, WorldLocation, ScreenPosition, false))
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
			{
				CanvasSlot->SetPosition(ScreenPosition);
			}
		}
	}
}

void UWIdget_HunStaminaGauge::UpdateRoundGauge(float GaugeValue)
{
	if (IsValid(DynamicRoundGaugeMaterial))
	{
		DynamicRoundGaugeMaterial->SetScalarParameterValue(FName("Percent"), GaugeValue);
	}
}

void UWIdget_HunStaminaGauge::OnStaminaUpdated(float CurrentStamina, float MaxStamina)
{
	if (MaxStamina > 0.0f)
	{
		float Percentage = CurrentStamina / MaxStamina;
		UpdateRoundGauge(Percentage);
	}
}

void UWIdget_HunStaminaGauge::BindMoveComponent(UHun_MoveComponent* MovementComponent)
{
	if (MovementComponent)
	{
		TargetCharacter = MovementComponent->GetOwner();
		
		MovementComponent->OnStaminaUpdate.AddDynamic(this, &UWIdget_HunStaminaGauge::OnStaminaUpdated);
		OnStaminaUpdated(MovementComponent->CurrentStamina, MovementComponent->MaxStamina);
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
