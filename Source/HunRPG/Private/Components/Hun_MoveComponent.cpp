// Fill out your copyright notice in the Description page of Project Settings.


#include "HunRPG/Public/Components/Hun_MoveComponent.h"
#include "HunRPG/Public/System/HunRPG_StateTypes.h"
#include "HunRPG/Public/Components/Hun_StateComponent.h"
#include "HunRPG_DebugHelper.h"

#include "BlendSpaceAnalysis.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UHun_MoveComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	if (!IsValid(OwnerActor))
		return;

	OwnerCharacter = Cast<ACharacter>(OwnerActor);

	if (!IsValid(OwnerCharacter))
		return;

	MoveComponent = OwnerCharacter->GetCharacterMovement();
	StateComponent = OwnerCharacter->FindComponentByClass<UHun_StateComponent>();
}

void UHun_MoveComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

void UHun_MoveComponent::SetMoveSpeed_Interface_Implementation(FHun_ActionValue MoveSpeed, EHunRPG_ActionState State)
{
	if (!IsValid(MoveComponent))
		return;

	if (State == EHunRPG_ActionState::Idle || State == EHunRPG_ActionState::Moving)
	{
		MoveComponent->MaxWalkSpeed = MoveSpeed.WalkSpeed;
		StateComponent->SetState(State);
	}
	else if (State == EHunRPG_ActionState::Running)
	{
		MoveComponent->MaxWalkSpeed = MoveSpeed.RunSpeed;
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

void UHun_MoveComponent::DashInput_Interface_Implementation()
{
	if (!IsValid(OwnerCharacter) || !IsValid(MoveComponent) || !IsValid(StateComponent))
		return;

	EHunRPG_ActionState CurrentState = StateComponent->GetState();

	if (!CanJump()) 
	{
		return; 
	}
	
	FVector DashDirection = OwnerCharacter->GetActorForwardVector();
	float DashStrength = 2000.f;
	
	OwnerCharacter->LaunchCharacter(DashDirection * DashStrength, true, false);
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
