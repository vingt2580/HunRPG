// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Hun_Character.h"
#include "HunRPG_Core/Public/HunRPG_DebugHelper.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "Interface/Hun_MovementInterface.h"


// Sets default values
AHun_Character::AHun_Character()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;
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

	if (!CharacterData)
		return;

	UCharacterMovementComponent* MoveComponent = GetCharacterMovement();

	if (!MoveComponent)
		return;

	bUseControllerRotationYaw = false;
	
	MoveComponent->bOrientRotationToMovement = true; 
	MoveComponent->RotationRate = FRotator(0.f, 540.0f, 0.f);
	
	MoveComponent->MaxWalkSpeed = CharacterData->WalkSpeed;

	CachedMovementComponent = nullptr;
	
	TArray<UActorComponent*> Component;
	GetComponents(Component);
	for (UActorComponent* Comp : Component)
	{
		if (Comp && Comp->Implements<UHun_MovementInterface>())
		{
			CachedMovementComponent = Comp;
			HUN_LOG(FColor::Green,"Successfully cached movement component");
			break;
		}
	}
	if (CachedMovementComponent == nullptr)
	{
		HUN_LOG(FColor::Red,"Failed to cache movement component");
	}
}

void AHun_Character::Character_Move(FVector2D ActionValue)
{
	if (CachedMovementComponent == nullptr)
		return;
	if (!IsValid(CachedMovementComponent))
		return;
	
	IHun_MovementInterface::Execute_MovementInput_Interface(CachedMovementComponent, ActionValue);
}

