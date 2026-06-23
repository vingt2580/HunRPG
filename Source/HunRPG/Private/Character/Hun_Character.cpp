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

void AHun_Character::BeginPlay()
{
	Super::BeginPlay();
}

void AHun_Character::Character_Move(FVector2D ActionValue)
{
	if (!IsValid(CachedMovementComponent))
		return;
	TScriptInterface<IHun_MovementInterface> Interface = this;
	if (!IsValid(Interface.GetObject()))
		return;

	//Interface->MovementInput_Interface(ActionValue);
	IHun_MovementInterface::Execute_MovementInput_Interface(CachedMovementComponent, ActionValue);
}

void AHun_Character::Character_ResetMove()
{
	if (!IsValid(CachedMovementComponent))
		return;

	IHun_MovementInterface::Execute_ApplyStateSpeed_Interface(CachedMovementComponent, EHunRPG_ActionState::Idle);
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
	IHun_MovementInterface::Execute_ApplyStateSpeed_Interface(CachedMovementComponent, EHunRPG_ActionState::Running);
}

void AHun_Character::Character_Look(FVector2d LookAxisVector)
{
	if (!IsValid(Controller))
		return;

	float Sensitivity = 1.0f;

	if (ComponentsData)
	{
		//Sensitivity = MobData->MovementValue.LookSensitivity;
		//이걸 어디에 넣을지 고민
	}
	
	AddControllerYawInput(LookAxisVector.X * Sensitivity);
	AddControllerPitchInput(LookAxisVector.Y * -Sensitivity);
}

void AHun_Character::Character_Attack()
{
	if (!IsValid(CachedCombatComponent))
		return;
	
	IHun_CombatInterface::Execute_AttackInput_interface(CachedCombatComponent);
}


