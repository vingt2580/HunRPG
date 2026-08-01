// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_Grux.h"

#include "HunRPG_DebugHelper.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
