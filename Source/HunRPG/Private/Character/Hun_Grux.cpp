// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_Grux.h"

#include "HunRPG_DebugHelper.h"
#include "Character/Hun_Character.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/HunCollisionChannels.h"
#include "Engine/OverlapResult.h"
#include "System/HunRPG_AbilityTypes.h"


void AHun_Grux::ExecuteLeapAttack_BossPattern()
{
	if (!IsValid(CombatTarget))
		return;

	if (bIsLeaping)
		return;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = CombatTarget->GetActorLocation();
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

void AHun_Grux::ExecuteLAndingShockwave()
{
	float ShockwaveRadius = 800.0f;
	float ShockwaveDamage = 20.0f;
	float KnockbackPower = 1500.0f;

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

void AHun_Grux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
