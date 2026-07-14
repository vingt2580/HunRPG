// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Hun_Character.h"

#include <rapidjson/document.h>

#include "AutomationTestExcludelist.h"
#include "HunRPG_DebugHelper.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Hun_ActorComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/Hun_CombatInterface.h"

#include "Interface/Hun_MovementInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AHun_Character::AHun_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

	bIsLockedOn = false;
	CurrentLockOnTarget = nullptr;
}

void AHun_Character::BeginPlay()
{
	Super::BeginPlay();

}

void AHun_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeadZoneLockOn(DeltaTime);
}

void AHun_Character::ToggleLockOn()
{
	if (bIsLockedOn)
	{
		bIsLockedOn = false;
		CurrentLockOnTarget = nullptr;
		HUN_LOG(FColor::Red, "락온 해제");
	}
	else
	{
		AActor* FindedTarget = FindLockOnTarget();
		if (IsValid(FindedTarget))
		{
			CurrentLockOnTarget = FindedTarget;
			bIsLockedOn = true;
			HUN_LOG(FColor::Red, "락온 성공 : %s", *CurrentLockOnTarget.GetName());
		}
	}
}

void AHun_Character::ForceSetLockOnTarget(AActor* NewLockOnTarget)
{
	if (IsValid(NewLockOnTarget))
	{
		CurrentLockOnTarget = NewLockOnTarget;
		bIsLockedOn = true;
	}
}

void AHun_Character::ClearLockOnTarget()
{
	bIsLockedOn = false;
	CurrentLockOnTarget = nullptr;
}

AActor* AHun_Character::FindLockOnTarget() const
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams ObjectQueryParams;

	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	bool bLockOnTarget = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,ObjectQueryParams,
		FCollisionShape::MakeSphere(LockOnRadius),
		CollisionQueryParams);

	if (!bLockOnTarget)
		return nullptr;

	AActor* BestTarget = nullptr;
	float ClosestDistance = LockOnRadius;

	FVector CameraForward = GetController()->GetControlRotation().Vector();

	for (FOverlapResult& Result : OverlapResults)
	{
		AActor* LockonActor = Result.GetActor();
		if (!LockonActor)
			continue;

		FVector DirectiontoTarget = (LockonActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float DotProduct =FVector::DotProduct(CameraForward,DirectiontoTarget);

		if (DotProduct > 0.f)
		{
			float Distance = FVector::Dist(GetActorLocation(), LockonActor->GetActorLocation());

			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				BestTarget = LockonActor;
			}
		}
	}

	return BestTarget;
}

void AHun_Character::DeadZoneLockOn(const float DeltaTime)
{
	if (bIsLockedOn && IsValid(CurrentLockOnTarget))
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (IsValid(PC))
		{
			FRotator CurrentCameraRot = PC->GetControlRotation();
			FVector TargetLocation = CurrentLockOnTarget->GetActorLocation();

			FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(PC->PlayerCameraManager->GetCameraLocation(), TargetLocation);
			FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(LookAtRot, CurrentCameraRot);

			float TargetYaw = CurrentCameraRot.Yaw;
			float TargetPitch = CurrentCameraRot.Pitch;

			if (FMath::Abs(DeltaRot.Yaw) > DeadZoneYaw)
			{
				float ExcessYaw = 0.f;
				if (DeltaRot.Yaw > 0)
					ExcessYaw = DeltaRot.Yaw - DeadZoneYaw;
				else
					ExcessYaw = DeltaRot.Yaw + DeadZoneYaw;

				TargetYaw += ExcessYaw;
			}

			if (FMath::Abs(DeltaRot.Pitch) > DeadzonePitch)
			{
				float ExcessPitch = 0.f;
				if (DeltaRot.Pitch > 0)
					ExcessPitch = DeltaRot.Pitch - DeadzonePitch;
				else
					ExcessPitch = DeltaRot.Pitch + DeadzonePitch;

				TargetPitch += ExcessPitch;
			}

			FRotator FinalTargetRot = FRotator(TargetPitch, TargetYaw, 0.f);
			FRotator SmoothRot = FMath::RInterpTo(CurrentCameraRot, FinalTargetRot, DeltaTime, 5.f);

			PC->SetControlRotation(SmoothRot);
		}

		float Distance = FVector::Dist(GetActorLocation(), CurrentLockOnTarget->GetActorLocation());
		if (Distance > LockOnRadius * 1.5f)
			ToggleLockOn();
	}
	else if (bIsLockedOn && !IsValid(CurrentLockOnTarget))
	{
		ToggleLockOn();
	}
}

void AHun_Character::Character_Move(FVector2D ActionValue)
{
	if (!IsValid(CachedMovementComponent))
		return;
	TScriptInterface<IHun_MovementInterface> Interface = this;
	if (!IsValid(Interface.GetObject()))
		return;
	
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

	if (bIsLockedOn)
		return;
	
	float Sensitivity = 1.0f;
	
	AddControllerYawInput(LookAxisVector.X * Sensitivity);
	AddControllerPitchInput(LookAxisVector.Y * -Sensitivity);
}

void AHun_Character::Character_Attack()
{
	if (!IsValid(CachedCombatComponent))
		return;
	
	IHun_CombatInterface::Execute_AttackInput_interface(CachedCombatComponent, FindLockOnTarget());
}

void AHun_Character::Character_Ability(EHun_AbilityType AbilityType)
{
	if (!IsValid(CachedCombatComponent))
		return;

	IHun_CombatInterface::Execute_ActiveAbility_Interface(CachedCombatComponent, AbilityType);
}


