// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_MobBase.h"

#include "Components/CapsuleComponent.h"
#include "HunRPG_DebugHelper.h"


// Sets default values
AHun_MobBase::AHun_MobBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHun_MobBase::BeginPlay()
{
	Super::BeginPlay();

	if (!MobData)
		return;
	
	CurrentHealthPoint = MobData->MaxHealthPoint;
}

// Called every frame
void AHun_MobBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AHun_MobBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float TakenDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CheckHitAngle(DamageCauser);
	PlayHitAnimation();

	CurrentHealthPoint -= TakenDamage;
	HUN_LOG(FColor::Magenta, "TakeDamage: %f", TakenDamage);
	
	if (CurrentHealthPoint <= 0.0f || IsDeath)
	{
		Die(DamageCauser);
		return 0.0f;
	}
	
	return TakenDamage;
	
}

void AHun_MobBase::Die(AActor* DamageCauser)
{
	PlayDeathAnimation();

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetLifeSpan(1.6f);
}

void AHun_MobBase::CheckHitAngle(AActor* DamageCauser)
{
	if (CurrentHealthPoint > 0.0f && DamageCauser)
	{
		FVector HitDirection = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		float ForwardDot = FVector::DotProduct(GetActorForwardVector(), HitDirection);
		float RightDot = FVector::DotProduct(GetActorRightVector(), HitDirection);
		
		HitAngle = FMath::RadiansToDegrees(FMath::Atan2(RightDot, ForwardDot));

		HUN_LOG(FColor::Red, "HitAngle: %f", HitAngle);
		
		IsHit = true;
	}
}

void AHun_MobBase::PlayHitAnimation()
{
	if (!IsHit)
		return;
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		FName SectionName = TEXT("Front");

		if (HitAngle >= -45.0f && HitAngle <= 45.0f) 
			SectionName = TEXT("Front");
		else if (HitAngle > 45.0f && HitAngle <= 135.0f) 
			SectionName = TEXT("Right");
		else if (HitAngle >= -135.0f && HitAngle < -45.0f) 
			SectionName = TEXT("Left");
		else 
			SectionName = TEXT("Back");
		
		UAnimMontage* HitReactionMontage = MobData->HitReactionMontage;
		
		if (HitReactionMontage)
		{
			AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
			IsHit = false;
		}
	}
}

void AHun_MobBase::PlayDeathAnimation()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		FName SectionName = TEXT("Death_Front");

		if (HitAngle >= -180.0f && HitAngle <= 180.0f) 
			SectionName = TEXT("Death_Back");
		else
			SectionName = TEXT("Death_Front");
		
		UAnimMontage* DeathMontage = MobData->DeathMontage;
		
		if (DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
			IsDeath = true;
		}
	}
}


