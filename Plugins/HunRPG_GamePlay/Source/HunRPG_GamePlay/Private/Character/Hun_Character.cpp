// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Hun_Character.h"

#include <rapidjson/document.h>

#include "HunRPG_Core/Public/HunRPG_DebugHelper.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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
	
	GetCharacterMovement()->bOrientRotationToMovement =true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,500.0f,0.0f);

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

	if (!IsValid(CharacterData))
		return;

	for (TSubclassOf<UActorComponent> ComponentClass : CharacterData->CharacterComponents)
	{
		if (ComponentClass)
		{
			UActorComponent* NewComponent = NewObject<UActorComponent>(this, ComponentClass);
			NewComponent->RegisterComponent();
		}
	}
}

void AHun_Character::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* MoveComponent = GetCharacterMovement();

	if (!MoveComponent)
		return;

	bUseControllerRotationYaw = false;
	
	MoveComponent->bOrientRotationToMovement = true; 
	MoveComponent->RotationRate = FRotator(0.f, 540.0f, 0.f);
	MoveComponent->MaxWalkSpeed = CharacterData->MovementValue.WalkSpeed;
	
	CachComponent();
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

	IHun_MovementInterface::Execute_SetMoveSpeed_Interface(CachedMovementComponent, CharacterData->MovementValue, EHunRPG_ActionState::Idle);
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
	IHun_MovementInterface::Execute_SetMoveSpeed_Interface(CachedMovementComponent,CharacterData->MovementValue, EHunRPG_ActionState::Running);
}

void AHun_Character::Character_Look(FVector2d LookAxisVector)
{
	if (!IsValid(Controller))
		return;

	float Sensitivity = 1.0f;

	if (CharacterData)
	{
		Sensitivity = CharacterData->MovementValue.LookSensitivity;
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

void AHun_Character::CachComponent()
{
	if (!CharacterData)
		return;

	CachedMovementComponent = nullptr;
	
	TArray<UActorComponent*> Component;
	GetComponents(Component);
	for (UActorComponent* Comp : Component)
	{
		if (!CachedMovementComponent && Comp->Implements<UHun_MovementInterface>())
		{
			CachedMovementComponent = Comp;
			HUN_LOG(FColor::Green,"Successfully cached movement component");
		}

		if (!CachedCombatComponent && Comp->Implements<UHun_CombatInterface>())
		{
			CachedCombatComponent = Comp;
			HUN_LOG(FColor::Green, "Successfully cached combat component");
		}

		if (CachedMovementComponent && CachedCombatComponent)
		{
			break;
		}
		
	}
	if (CachedMovementComponent == nullptr)
		HUN_LOG(FColor::Red, "Failed to cache movement component");
	if (CachedCombatComponent == nullptr)
		HUN_LOG(FColor::Red, "Failed to cache combat component");
}


