// Fill out your copyright notice in the Description page of Project Settings.


#include "Hun_Components/ActionComponents/Movement/Public/Hun_MoveComponent.h"
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
	
}


void UHun_MoveComponent::MovementInput_Interface_Implementation(FVector2D MoveVector)
{
	if (!OwnerCharacter && OwnerCharacter->Controller)
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

void UHun_MoveComponent::SetMoveSpeed_Interface_Implementation(float MoveSpeed)
{
	if (!MoveComponent)
		return;

	MoveComponent->MaxWalkSpeed = MoveSpeed;
}
