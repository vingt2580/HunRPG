// Fill out your copyright notice in the Description page of Project Settings.


#include "HunRPG/Public/Components/Hun_MoveComponent.h"
#include "HunRPG/Public/System/HunRPG_StateTypes.h"
#include "HunRPG/Public/Components/Hun_StateComponent.h"
#include "HunRPG_DebugHelper.h"

#include "BlendSpaceAnalysis.h"
#include "Data/Hun_CharacterData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UHun_MoveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(OwnerCharacter))
		return;

	MoveComponent = OwnerCharacter->GetCharacterMovement();
	StateComponent = OwnerCharacter->FindComponentByClass<UHun_StateComponent>();

	MaxSpeed = GetMobData()->MovementValue.WalkSpeed;
	MoveComponent->MaxWalkSpeed = GetMobData()->MovementValue.WalkSpeed;
}

void UHun_MoveComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	EHunRPG_ActionState CurrentState = StateComponent->GetState();
	
	if (bIsRunning)
	{
		if (CurrentStamina > 0.0f)
		{
			CurrentStamina = FMath::Max(0.0f, CurrentStamina - (10.0f * DeltaTime));
			TimeSinceLastStaminaUse = 0.0f;

			if (OnStaminaUpdate.IsBound())
			{
				OnStaminaUpdate.Broadcast(CurrentStamina, MaxStamina);
			}
		}
		else
		{
			bIsRunning = false;
			HUN_LOG(FColor::Red, "Stamina Not Enough!!!");
		}
	}
	else if (CurrentState == EHunRPG_ActionState::Idle && CurrentStamina < MaxStamina)
	{
		TimeSinceLastStaminaUse += DeltaTime;

		if (TimeSinceLastStaminaUse >= StaminaReganDelay)
		{
			CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + (StaminaReganFigure * DeltaTime));
			
			if (OnStaminaUpdate.IsBound())
			{
				OnStaminaUpdate.Broadcast(CurrentStamina, MaxStamina);
			}
		}
	}
}


UHun_MoveComponent::UHun_MoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaReganFigure = 10.0f; 
	StaminaReganDelay = 1.5f; 
	TimeSinceLastStaminaUse = 0.0f;
	bIsRunning = false;
}

void UHun_MoveComponent::MovementInput_Interface_Implementation(FVector2D MoveVector)
{
	if (!IsValid(OwnerCharacter))
		return;

	if (!CanMove())
		return;

	const FRotator MovementRotation(0.f,OwnerCharacter->Controller->GetControlRotation().Yaw,0.f);
	
	if (MoveVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		OwnerCharacter->AddMovementInput(ForwardDirection,MoveVector.Y);
	}

	if (MoveVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		OwnerCharacter->AddMovementInput(RightDirection,MoveVector.X);
	}
}

void UHun_MoveComponent::ApplyStateSpeed_Interface_Implementation(EHunRPG_ActionState State)
{
	if (!IsValid(MoveComponent))
		return;

	if (State == EHunRPG_ActionState::Idle || State == EHunRPG_ActionState::Moving)
	{
		MoveComponent->MaxWalkSpeed = GetMobData()->MovementValue.WalkSpeed;
		StateComponent->SetState(State);
	}
	else if (State == EHunRPG_ActionState::Running)
	{
		MoveComponent->MaxWalkSpeed = GetMobData()->MovementValue.RunSpeed;
		StateComponent->SetState(State);
	}
}

void UHun_MoveComponent::JumpInput_interface_Implementation()
{
	if (!IsValid(OwnerCharacter) || !IsValid(MoveComponent) || !IsValid(StateComponent))
		return;
	
	EHunRPG_ActionState CurrentState = StateComponent->GetState();
	
	if (!CanJump()) 
	{
		return; 
	}

	if (ConsumeStamina(20.0f))
	{
		if (CurrentState == EHunRPG_ActionState::Running)
		{
			MoveComponent->AirControl = 0.8f;
			HUN_LOG(FColor::Yellow, "Moving Jumping");
		}
		else if (CurrentState == EHunRPG_ActionState::Idle || CurrentState == EHunRPG_ActionState::Moving)
		{
			MoveComponent->StopMovementImmediately();
			MoveComponent->AirControl = 0.2f;
        
			HUN_LOG(FColor::Yellow, "Inplace Jumping");
		}
	
		StateComponent->SetState(EHunRPG_ActionState::Jumping);
		OwnerCharacter->Jump();
	}
}

void UHun_MoveComponent::DashInput_Interface_Implementation()
{
	if (!IsValid(OwnerCharacter) || !IsValid(MoveComponent) || !IsValid(StateComponent))
		return;

	EHunRPG_ActionState CurrentState = StateComponent->GetState();

	if (!CanJump()) 
		return; 

	if (ConsumeStamina(10.0f))
	{
		FVector DashDirection = OwnerCharacter->GetActorForwardVector();
	
		OwnerCharacter->LaunchCharacter(DashDirection * GetMobData()->MovementValue.DashLength, true, false);	
	}
}

void UHun_MoveComponent::SetMoveSpeed_Interface_Implementation(float Speed)
{
	if (!IsValid(OwnerCharacter) || !IsValid(MoveComponent))
		return;

	MoveComponent->MaxWalkSpeed = Speed;
}

void UHun_MoveComponent::InitializeMobData()
{
	Super::InitializeMobData();
}

bool UHun_MoveComponent::CanJump()
{
	EHunRPG_ActionState CurrentState = StateComponent->GetState();

	if (CurrentState == EHunRPG_ActionState::Jumping || 
		CurrentState == EHunRPG_ActionState::Falling ||
		CurrentState == EHunRPG_ActionState::Attacking || 
		CurrentState == EHunRPG_ActionState::HitReaction || 
		CurrentState == EHunRPG_ActionState::Dead) 
	{
		return false; 
	}
	return true;
}

bool UHun_MoveComponent::CanMove()
{
	EHunRPG_ActionState CurrentState = StateComponent->GetState();

	if (CurrentState == EHunRPG_ActionState::Falling ||
		CurrentState == EHunRPG_ActionState::Attacking || 
		CurrentState == EHunRPG_ActionState::HitReaction || 
		CurrentState == EHunRPG_ActionState::Dead) 
	{
		return false; 
	}
	return true;
}

bool UHun_MoveComponent::ConsumeStamina(float Value)
{
	if (CurrentStamina >= Value)
	{
		CurrentStamina -= Value;
		TimeSinceLastStaminaUse = 0.0f;

		if (OnStaminaUpdate.IsBound())
		{
			OnStaminaUpdate.Broadcast(CurrentStamina, MaxStamina);
		}
		HUN_LOG(FColor::Red, "CurrentStamina: %f", CurrentStamina);
		return true;
	}
	return false;
}

void UHun_MoveComponent::SetRunningState(bool bRunning)
{
	bIsRunning = bRunning;
	if (bIsRunning)
	{
		TimeSinceLastStaminaUse = 0.0f;
	}
}
