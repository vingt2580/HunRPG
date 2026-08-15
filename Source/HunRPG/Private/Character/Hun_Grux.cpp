// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_Grux.h"

#include "HunRPG_DebugHelper.h"
#include "Actor/Hun_ChainExplosion_Projectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "Character/Hun_Character.h"
#include "Components/Hun_MoveComponent.h"
#include "Controller/AIController/Hun_MonsterBaseAIController.h"
#include "Data/Hun_CharacterData.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/HunCollisionChannels.h"
#include "Engine/OverlapResult.h"
#include "System/HunRPG_AbilityTypes.h"


void AHun_Grux::ExecuteLeapAttack_BossPattern()
{
	if (!IsValid(CombatTargetPC))
		return;

	if (bIsLeaping)
		return;

	if (APawn* TargetPawn = CombatTargetPC->GetPawn())
	{
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = TargetPawn->GetActorLocation();
		FVector LaunchVelocity;

		float LeapHighValue = 0.5f;

		bool bSucces = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this,
			LaunchVelocity,
			StartLocation,
			EndLocation,
			GetWorld()->GetGravityZ(),
			LeapHighValue);

		if (bSucces)
		{
			bIsLeaping = true;
			HUN_LOG(FColor::Blue, "bIsLeaping True");
			LaunchCharacter(LaunchVelocity, true, true);

			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
	}
}

void AHun_Grux::ExecuteLandingShockwave()
{
	FVector BossLocation = GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams OverlapObjectQueryParams;
	OverlapObjectQueryParams.AddObjectTypesToQuery(ECC_HunCharacter);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		BossLocation,
		FQuat::Identity,
		OverlapObjectQueryParams,
		FCollisionShape::MakeSphere(ShockwaveRadius),
		QueryParams);

	if (bHit)
	{
		TSet<AActor*> DamagedActors;
		
		for (const FOverlapResult& Result : OverlapResults)
		{
			AHun_Character* HitCharacter = Cast<AHun_Character>(Result.GetActor());
			
			if (IsValid(HitCharacter) && !DamagedActors.Contains(HitCharacter))
			{
				DamagedActors.Add(HitCharacter);
				
				FHun_AbilityInfo AbilityInfo;
				FDamageEvent DamageEvent(AbilityInfo.DamageType);
				
				HitCharacter->TakeDamage(
					ShockwaveDamage,
					DamageEvent,
					GetController(),
					this);

				FVector KnockbackDirection = (HitCharacter->GetActorLocation() - BossLocation).GetSafeNormal();

				KnockbackDirection.Z = 0.5f;
				KnockbackDirection.Normalize();

				FVector KnockbackVelocity = KnockbackDirection * KnockbackPower;

				HitCharacter->LaunchCharacter(KnockbackVelocity, true, true);
			}
		}
	}
	DrawDebugSphere(GetWorld(), BossLocation, ShockwaveRadius, 24, FColor::Red, false, 2.0f);
}

void AHun_Grux::ExecuteChainExplosion()
{
	if (!IsValid(ChainExplosionClass))
		return;

	FVector StartLocation = GetActorLocation() +  (GetActorForwardVector() * 150.f);
	StartLocation.Z -= 180.f;

	AHun_ChainExplosion_Projectile* ChainExplosion = GetWorld()->SpawnActor<AHun_ChainExplosion_Projectile>(ChainExplosionClass, StartLocation, GetActorRotation());

	if (IsValid(ChainExplosion))
	{
		ChainExplosion->SetInstigator(this);

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		ChainExplosion->DesignateAtTarget(PC);
		ChainExplosion->MaxExplosionsCount = 0;
	}
}

void AHun_Grux::ExecuteStampedStart()
{
	if (!bIsStampeding)
	{
		UHun_MoveComponent* MoveComp = FindComponentByClass<UHun_MoveComponent>();
		if (!IsValid(MoveComp))
			return;

		IHun_MovementInterface::Execute_SetMoveSpeed_Interface(MoveComp, 800.f);
		bIsStampeding = true;
	}
}

void AHun_Grux::ExecuteStampedKnockup()
{
	FVector HitLocation = GetActorLocation() +  (GetActorForwardVector() * 500.f);

	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams OverlapObjectQueryParams;
	OverlapObjectQueryParams.AddObjectTypesToQuery(ECC_HunCharacter);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		HitLocation,
		FQuat::Identity,
		OverlapObjectQueryParams,
		FCollisionShape::MakeCapsule(StampedRadius, StampedHeight),
		QueryParams);

	if (bHit)
	{
		TSet<AActor*> DamagedActors;

		for (const FOverlapResult& Result : OverlapResults)
		{
			AHun_Character* HitCharacter = Cast<AHun_Character>(Result.GetActor());
			if (IsValid(HitCharacter) && !DamagedActors.Contains(HitCharacter))
			{
				DamagedActors.Add(HitCharacter);
				
				FHun_AbilityInfo AbilityInfo;
				FDamageEvent DamageEvent(AbilityInfo.DamageType);
				
				HitCharacter->TakeDamage(
					StampedDamage,
					DamageEvent,
					GetController(),
					this);

				FVector KnockUpDirection = FVector(0, 0, 1.f);

				FVector KnockbackVelocity = KnockUpDirection * KnockUpPower;

				HitCharacter->LaunchCharacter(KnockbackVelocity, false, true);
			}
		}
	}
	DrawDebugCapsule(GetWorld(), HitLocation, StampedHeight, StampedRadius, FQuat::Identity, FColor::Red, false, 2.0f);
}

void AHun_Grux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStampeding)
	{
		FVector DirectionToTarget = (CombatTargetPC->GetPawn()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddMovementInput(DirectionToTarget, 1.0f);

		float Distance = FVector::Dist(GetActorLocation(), CombatTargetPC->GetPawn()->GetActorLocation());
		if (Distance <= 500.f)
		{
			bIsStampeding = false;
			ChangeStqmpedKnockupMontage();
			
			UHun_MoveComponent* MoveComp = FindComponentByClass<UHun_MoveComponent>();
			if (!IsValid(MoveComp))
				return;

			IHun_MovementInterface::Execute_SetMoveSpeed_Interface(MoveComp, CharacterData->MovementValue.WalkSpeed);
		}

		float CurrentMoveSpeed = GetMovementComponent()->GetMaxSpeed();
		HUN_LOG(FColor::Yellow, "Movement speed: %f", CurrentMoveSpeed);
	}
}

void AHun_Grux::Landed(const FHitResult& Hit)
{
	Super::OnLanded(Hit);

	if (bIsLeaping)
	{
		bIsLeaping = false;	
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance))
		{
			AnimInstance->Montage_SetNextSection("LeapStart","Leap");
			HUN_LOG(FColor::Green, "착지확인 랜딩 몽타주 전환");
		}
	}
}

void AHun_Grux::ChangeStqmpedKnockupMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_SetNextSection("Stamped_Start","Stameped_Knockup");
	}
}

