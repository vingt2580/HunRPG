// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

#include "HunRPG_GamePlay/Public/PlayerController/Hun_PlayerController.h"

void AHun_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AHun_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHun_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

	if (IsValid(InputActions))
	{
		PEI->BindAction(InputActions->Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	}
}

void AHun_PlayerController::Input_Move(const FInputActionValue& ActionValue)
{
	FVector2D MovementVector = ActionValue.Get<FVector2D>();
}
