// Fill out your copyright notice in the Description page of Project Settings.


#include "Hun_Components/ActionComponents/Movement/Public/Hun_MoveComponent.h"
#include "HunRPG_Core/Public/System/HunRPG_StateTypes.h"
#include "BlendSpaceAnalysis.h"
#include "HunRPG_DebugHelper.h"
#include "Hun_Components/ActionComponents/State/Public/Hun_StateComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UHun_MoveComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	if (!OwnerActor)
		return;

	OwnerCharacter = Cast<ACharacter>(OwnerActor);

	if (!OwnerCharacter)
		return;

	MoveComponent = OwnerCharacter->GetCharacterMovement();
	StateComponent = OwnerCharacter->FindComponentByClass<UHun_StateComponent>();
	
}


void UHun_MoveComponent::MovementInput_Interface_Implementation(FVector2D MoveVector)
{
	if (!OwnerCharacter)
		return;

	const FRotator MovementRotation(0.f,OwnerCharacter->Controller->GetControlRotation().Yaw,0.f);

	if (MoveVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		OwnerCharacter->AddMovementInput(ForwardDirection,MoveVector.Y);
		HUN_LOG(FColor::Green, "Moving Character %s", *MoveVector.ToString());
	}

	if (MoveVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		OwnerCharacter->AddMovementInput(RightDirection,MoveVector.X);
		HUN_LOG(FColor::Green, "Moving Character %s", *MoveVector.ToString());
	}

	if (!StateComponent)
		return;
	StateComponent->SetState(EHunRPG_ActionState::Moving);
}

void UHun_MoveComponent::SetMoveSpeed_Interface_Implementation(float MoveSpeed)
{
	if (!MoveComponent)
		return;

	MoveComponent->MaxWalkSpeed = MoveSpeed;
}

void UHun_MoveComponent::JumpInput_interface_Implementation()
{
	if (!OwnerCharacter || !MoveComponent || !StateComponent)
		return;
	
	EHunRPG_ActionState CurrentState = StateComponent->GetState();

	if (CurrentState == EHunRPG_ActionState::Jumping || 
		CurrentState == EHunRPG_ActionState::Falling ||
		CurrentState == EHunRPG_ActionState::Attacking || 
		CurrentState == EHunRPG_ActionState::HitReaction || 
		CurrentState == EHunRPG_ActionState::Dead) // 이거 맞나..? 개선방안 있는지 확인
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
		FVector CurrentVelocity = MoveComponent->Velocity;
		MoveComponent->Velocity = FVector(0.f, 0.f, CurrentVelocity.Z);

		MoveComponent->AirControl = 0.2f;
        
		HUN_LOG(FColor::Yellow, "Inplace Jumping");
	}
	StateComponent->SetState(EHunRPG_ActionState::Jumping);
	OwnerCharacter->Jump();
}

void UHun_MoveComponent::DashInput_Interface_Implementation()
{
	if (!OwnerCharacter || !MoveComponent || !StateComponent)
		return;
	
	FVector DashDirection = OwnerCharacter->GetActorForwardVector();
	float DashStrength = 2000.f;
	
	OwnerCharacter->LaunchCharacter(DashDirection * DashStrength, true, false);
	
	StateComponent->SetState(EHunRPG_ActionState::Running);
	
	MoveComponent->MaxWalkSpeed = 600.f; 

	UE_LOG(LogTemp, Log, TEXT("대시 실행! 현재 상태: Running"));
}
