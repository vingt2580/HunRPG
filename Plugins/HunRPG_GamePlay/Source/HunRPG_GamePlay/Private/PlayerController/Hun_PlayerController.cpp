// Fill out your copyright notice in the Description page of Project Settings.

#include "HunRPG_GamePlay/Public/PlayerController/Hun_PlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

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
		PEI->BindAction(InputActions->Input_Move, ETriggerEvent::Completed, this, &ThisClass::Reset_MoveSpeed);
		PEI->BindAction(InputActions->Input_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
		PEI->BindAction(InputActions->Input_Dash, ETriggerEvent::Started, this, &ThisClass::Input_Dash);
		PEI->BindAction(InputActions->Input_Look, ETriggerEvent::Triggered ,this, &ThisClass::Input_CameraLook);
		PEI->BindAction(InputActions->Input_Attack, ETriggerEvent::Started, this, &ThisClass::Input_Attack);
	}
}

void AHun_PlayerController::Input_Move(const FInputActionValue& ActionValue)
{
	FVector2D MovementVector = ActionValue.Get<FVector2D>();
	HunCharacter->Character_Move(MovementVector);
}

void AHun_PlayerController::Input_Jump()
{
	HunCharacter->Character_Jump();
}

void AHun_PlayerController::Input_Dash()
{
	HunCharacter->Character_Dash();
}

void AHun_PlayerController::Input_CameraLook(const FInputActionValue& ActionValue)
{
	FVector2D LookAxisValue = ActionValue.Get<FVector2D>();
	HunCharacter->Character_Look(LookAxisValue);
}

void AHun_PlayerController::Input_Attack()
{
	HunCharacter->CHaracter_Attack();
}

void AHun_PlayerController::Reset_MoveSpeed()
{
	HunCharacter->Character_ResetMove();
}
