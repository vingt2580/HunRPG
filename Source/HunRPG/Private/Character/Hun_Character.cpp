// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Hun_Character.h"

#include <rapidjson/document.h>

#include "HunRPG_DebugHelper.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Hun_ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/Hun_CombatInterface.h"

#include "Interface/Hun_MovementInterface.h"


// Sets default values
AHun_Character::AHun_Character()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SocketOffset = FVector(0.0f,0.0, 80.0f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;
}

void AHun_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!IsValid(MobData))
		return;

	for (TSubclassOf<UHun_ActorComponent> ComponentClass : MobData->CharacterComponents)
	{
		if (ComponentClass)
		{
			UHun_ActorComponent* NewComponent = NewObject<UHun_ActorComponent>(this, ComponentClass);
			NewComponent->RegisterComponent();

			if (!IsValid(CachedMovementComponent) && NewComponent->Implements<UHun_MovementInterface>())
			{
				CachedMovementComponent = NewComponent;
				HUN_LOG(FColor::Green,"Successfully cached movement component");
			}

			if (!IsValid(CachedCombatComponent) && NewComponent->Implements<UHun_CombatInterface>())
			{
				CachedCombatComponent = NewComponent;
				HUN_LOG(FColor::Green, "Successfully cached combat component");
			}
		}
	}

	if (!IsValid(CachedMovementComponent))
		HUN_LOG(FColor::Red, "Failed to create and cache movement component");
	if (!IsValid(CachedMovementComponent))
		HUN_LOG(FColor::Red, "Failed to create and cache combat component");
}

void AHun_Character::BeginPlay()
{
	Super::BeginPlay();

	IHun_CombatInterface::Execute_InitializeCombatData_Interface(CachedCombatComponent, MobData->CombatValue);
}

void AHun_Character::Character_Move(FVector2D ActionValue)
{
	if (!IsValid(CachedMovementComponent))
		return;
	
	IHun_MovementInterface::Execute_MovementInput_Interface(CachedMovementComponent, ActionValue);
}

void AHun_Character::Character_ResetMove()
{
	if (!IsValid(CachedMovementComponent))
		return;

	IHun_MovementInterface::Execute_SetMoveSpeed_Interface(CachedMovementComponent, MobData->MovementValue, EHunRPG_ActionState::Idle);
}

void AHun_Character::Character_Jump()
{
	if (!IsValid(CachedMovementComponent))
		return;

	IHun_MovementInterface::Execute_JumpInput_interface(CachedMovementComponent);
}

void AHun_Character::Character_Dash()
{
	if (!IsValid(CachedMovementComponent))
		return;

	IHun_MovementInterface::Execute_DashInput_Interface(CachedMovementComponent);
	IHun_MovementInterface::Execute_SetMoveSpeed_Interface(CachedMovementComponent,MobData->MovementValue, EHunRPG_ActionState::Running);
}

void AHun_Character::Character_Look(FVector2d LookAxisVector)
{
	if (!IsValid(Controller))
		return;

	float Sensitivity = 1.0f;

	if (MobData)
	{
		Sensitivity = MobData->MovementValue.LookSensitivity;
	}
	
	AddControllerYawInput(LookAxisVector.X * Sensitivity);
	AddControllerPitchInput(LookAxisVector.Y * Sensitivity);
}

void AHun_Character::CHaracter_Attack()
{
	if (!IsValid(CachedCombatComponent))
		return;
	
	IHun_CombatInterface::Execute_AttackInput_interface(CachedCombatComponent);
}


