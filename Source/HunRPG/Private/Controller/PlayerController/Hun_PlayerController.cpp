// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/PlayerController/Hun_PlayerController.h"
#include "Character/Hun_Character.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

AHun_PlayerController::AHun_PlayerController()
{
	TeamId = FGenericTeamId(0);
}

FGenericTeamId AHun_PlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void AHun_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddtoViewportHUD();
}

void AHun_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHun_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//HunCharacter = GetPawn<AHun_Character>();

	//if (!IsValid(HunCharacter))
		//return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(PEI))
		return;
	
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
	if (!IsValid(HunCharacter))
		return;
	
	FVector2D MovementVector = ActionValue.Get<FVector2D>();
	HunCharacter->Character_Move(MovementVector);
}

void AHun_PlayerController::Input_Jump()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Jump();
}

void AHun_PlayerController::Input_Dash()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Dash();
}

void AHun_PlayerController::Input_CameraLook(const FInputActionValue& ActionValue)
{
	if (!IsValid(HunCharacter))
		return;
	
	FVector2D LookAxisValue = ActionValue.Get<FVector2D>();
	HunCharacter->Character_Look(LookAxisValue);
}

void AHun_PlayerController::Input_Attack()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Attack();
}

void AHun_PlayerController::Reset_MoveSpeed()
{
	if (!IsValid(HunCharacter))
		
		return;
	HunCharacter->Character_ResetMove();
}

void AHun_PlayerController::AddtoViewportHUD()
{
	if (MainHUDWidget)
	{
		UWidget_HunHUD* MainHUD = CreateWidget<UWidget_HunHUD>(GetWorld(), MainHUDWidget);
		
		if (IsValid(MainHUD))
		{
			MainHUD->AddToViewport();
		}
	}
}
