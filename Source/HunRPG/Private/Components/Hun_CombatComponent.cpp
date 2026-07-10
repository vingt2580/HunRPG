#include "Components/Hun_CombatComponent.h"

#include "HunRPG/Public/System/HunRPG_StateTypes.h"
#include "HunRPG/Public/Components/Hun_StateComponent.h"
#include "HunRPG/Public/Data/Hun_CharacterData.h"
#include "HunRPG_DebugHelper.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Data/Hun_AbilityData.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#define COLLISIONCHANNEL_MONSTER ECC_EngineTraceChannel1

struct FDamageEvent;

UHun_CombatComponent::UHun_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHun_CombatComponent::AttackInput_interface_Implementation()
{
	if (!OwnerCharacter || !StateComponent)
		return;

	if (IsAttacking)
		return;

	StartComboAttack();
}

float UHun_CombatComponent::HunTakeDamage_interface_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(OwnerCharacter))
		return 0.0f;

	float TakeDamage = ApplyDamage(DamageAmount);

	IsHit = true;
	PlayHitAnimation();
	CheckHitAngle(DamageCauser);
	
	if (!IsAlive())
	{
		CharacterDie();
		return 0.0f;	
	}
	
	return TakeDamage;
}

void UHun_CombatComponent::ActiveAbility_Interface_Implementation(EHun_AbilityType AbilityType)
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!AnimInstance || !GetMobData()->CombatValue.ComboMontage)
		return;

	FName AbilitySection;

	switch (AbilityType)
	{
		case EHun_AbilityType::None: return;
		case EHun_AbilityType::Ability_A: AbilitySection = TEXT("Ability_A");  break;
		case EHun_AbilityType::Ability_B: AbilitySection = TEXT("Ability_B"); break;
		case EHun_AbilityType::Ultimate: AbilitySection = TEXT("Ultimate"); break;
	}

	StateComponent->SetState(EHunRPG_ActionState::Attacking);
	
	AnimInstance->Montage_Play(GetMobData()->CombatValue.AbilityMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(AbilitySection, GetMobData()->CombatValue.AbilityMontage);
}

void UHun_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(OwnerCharacter))
		return;

	MaxHealthPoint = GetMobData()->MaxHealthPoint;
	CurrentHealthPoint = GetMobData()->MaxHealthPoint;
	
	StateComponent = OwnerCharacter->FindComponentByClass<UHun_StateComponent>();
}

void UHun_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsWeaponTrace)
	{
		USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh();
		AttackWeaponTracing(MeshComp);
	}
}

bool UHun_CombatComponent::CanAttack()
{
	EHunRPG_ActionState CurrentState = StateComponent->GetState();

	if (CurrentState == EHunRPG_ActionState::HitReaction || 
		CurrentState == EHunRPG_ActionState::Dead) 
	{
		return false; 
	}
	return true;
}

void UHun_CombatComponent::StartComboAttack()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!AnimInstance || !GetMobData()->CombatValue.ComboMontage)
		return;
	
	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	OwnerCharacter->GetCharacterMovement()->AddImpulse(ForwardVector * GetMobData()->CombatValue.AttackMoveImpulse, true);
	
	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, GetMobData()->CombatValue.MaxComboCount);
	FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), CurrentComboCount));
	
	IsAttacking = true;
	StateComponent->SetState(EHunRPG_ActionState::Attacking);
	
	AnimInstance->Montage_Play(GetMobData()->CombatValue.ComboMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, GetMobData()->CombatValue.ComboMontage);
}

void UHun_CombatComponent::HitAttack()
{
	if (!IsValid(OwnerCharacter))
		return;
	
}

void UHun_CombatComponent::OnSaveCombo()
{
	IsAttacking = false;
	//HUN_LOG(FColor::Green, "End Attacking");
}

void UHun_CombatComponent::OnResetCombo()
{
	if (!IsValid(OwnerCharacter))
		return;
	
	IsAttacking = false;
	CurrentComboCount = 0;

	StateComponent->SetState(EHunRPG_ActionState::Idle);

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && ComboMontage)
	{
		AnimInstance->Montage_Stop(0.f, ComboMontage);
	}
}

void UHun_CombatComponent::OnTrace_Attack()
{
	if (!IsValid(OwnerCharacter))
		return;

	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();

	if (!IsValid(MeshComponent))
		return;

	IsWeaponTrace = true;

	WeaponStartPoint = MeshComponent->GetSocketLocation(TEXT("Socket_SwordStart"));
	WeaponEndPoint = MeshComponent->GetSocketLocation(TEXT("Socket_SwordEnd"));
}

void UHun_CombatComponent::OffTrace_Attack()
{
	IsWeaponTrace = false;
	AlreadyHitActors.Reset();
}

void UHun_CombatComponent::AttackWeaponTracing(USkeletalMeshComponent* MeshComponent)
{
	if (!IsWeaponTrace || !MeshComponent || !OwnerCharacter)
		return;

	FVector CurrentStartLoc = MeshComponent->GetSocketLocation(TEXT("Socket_SwordStart"));
	FVector CurrentEndLoc = MeshComponent->GetSocketLocation(TEXT("Socket_SwordEnd"));

	TArray<FHun_TraceLine> TraceLines;
	
	TraceLines.Emplace(WeaponStartPoint, CurrentStartLoc); 
	TraceLines.Emplace(WeaponEndPoint, CurrentEndLoc);   
	TraceLines.Emplace(WeaponStartPoint, CurrentEndLoc);  
	TraceLines.Emplace(WeaponEndPoint, CurrentStartLoc);  
	TraceLines.Emplace(CurrentStartLoc, CurrentEndLoc);

	int32 SegmentCount = 3;

	for (int32 i = 1; i < SegmentCount; i++)
	{
		float Alpha = static_cast<float>(i) / SegmentCount;

		const FVector PreviousMid = FMath::Lerp(WeaponStartPoint, WeaponEndPoint, Alpha);
		const FVector CurrentMid = FMath::Lerp(CurrentStartLoc, CurrentEndLoc, Alpha);

		TraceLines.Emplace(PreviousMid, CurrentMid);
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	FHitResult HitResult;
	
	for (const auto& Trace : TraceLines)
	{
		bool isHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Trace.StartPoint,
			Trace.EndPoint,
			ECC_Pawn,
			Params
		);

		if (isHit)
		{
			TObjectPtr<AActor> HitActor = HitResult.GetActor();
			if (HitActor&& !AlreadyHitActors.Contains(HitActor))
			{
				AlreadyHitActors.Add(HitActor);

				UGameplayStatics::ApplyDamage(
				HitActor,
				GetMobData()->CombatValue.BaseDamage,
				OwnerCharacter->GetController(),
				OwnerCharacter,
				UDamageType::StaticClass()
				);
			}
		}

		DrawDebugLine(GetWorld(), Trace.StartPoint, Trace.EndPoint, FColor::Red, false, 0.25);
	}

	WeaponStartPoint = CurrentStartLoc;
	WeaponEndPoint = CurrentEndLoc;
}

void UHun_CombatComponent::ExecuteAbility(const FHun_AbilityInfo& AbilityInfo, const FVector& ActivePoint, const FRotator& ActiveRot)
{
	AActor* OwnerActor = GetOwnerCharacter();
	if (!IsValid(OwnerActor))
	return;
	
	FVector OriginForward = ActiveRot.Vector();
	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor);

	bool bHit = false;

	if (AbilityInfo.Shape == EHun_AbilityShape::Circle || AbilityInfo.Shape == EHun_AbilityShape::Cone)
	{
		bHit = GetWorld()->OverlapMultiByObjectType(
			OverlapResults, ActivePoint, FQuat::Identity, 
			ObjectQueryParams, FCollisionShape::MakeSphere(AbilityInfo.Radius), CollisionParams);

		if (AbilityInfo.Shape == EHun_AbilityShape::Circle)
		{
			DrawDebugSphere(GetWorld(), ActivePoint, AbilityInfo.Radius, 20, FColor::Red, true, 1.5f);
		}
	}
	else if (AbilityInfo.Shape == EHun_AbilityShape::Box)
	{
		FVector BoxCenter = ActivePoint + (OriginForward * AbilityInfo.BoxExtend.X);
		bHit = GetWorld()->OverlapMultiByObjectType(
			OverlapResults, BoxCenter, ActiveRot.Quaternion(),
			ObjectQueryParams, FCollisionShape::MakeBox(AbilityInfo.BoxExtend), CollisionParams);

		DrawDebugBox(GetWorld(), ActivePoint, AbilityInfo.BoxExtend, FColor::Red, false, 1.5f);
	}

	if (bHit)
	{
		TSet<AActor*> HitResults;
		int32 HitCount = 0;
		
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			AActor* HitActor = OverlapResult.GetActor();

			if (!HitActor || HitResults.Contains(HitActor))
				continue;
			
			if (AbilityInfo.Shape == EHun_AbilityShape::Cone)
			{
				FVector DirectionToTarget = (HitActor->GetActorLocation() - ActivePoint).GetSafeNormal(); //적 방향구하기 (적 위치 - 내 위치 = 적의 방향(벡터)) + 정규화
				float DotProduct = FVector::DotProduct(OriginForward, DirectionToTarget); //적이 정면대비 얼마나 벗어났는지 확인 (정면 = 1, 옆 = 0, 뒤 = -1)
				float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(DotProduct)); //DotProduct로 얻은 코사인값을 각도Degress로 변환

				if (AngleToTarget > (AbilityInfo.ConeAngle / 2.0f))
					continue;
			}

			AHun_MobBase* HitMob = Cast<AHun_MobBase>(HitActor);
			
			if (!IsValid(HitMob))
				continue;

			HitResults.Add(HitMob);

			FDamageEvent DamageEvent(AbilityInfo.DamageType);

			HitMob->TakeDamage(
				AbilityInfo.AbilityBaseDamage,
				DamageEvent,
				OwnerActor->GetInstigatorController(),
				OwnerActor);

			HUN_LOG(FColor::Red, "HitMob Take Damage, Damage:%f, HitOwner: %s", AbilityInfo.AbilityBaseDamage, *HitMob->GetName());

			for (EHun_AbilityEffect Effect : AbilityInfo.Effects)
			{
				switch (Effect)
				{
					case EHun_AbilityEffect::None: break;
					case EHun_AbilityEffect::Brun: break;
					case EHun_AbilityEffect::Stun: break;
				}
			}

			HitCount++;
			if (AbilityInfo.MaxTargets > 0 && HitCount >= AbilityInfo.MaxTargets)
			{
				break;
			}
		}
	}
}

void UHun_CombatComponent::PlayHitAnimation()
{
	if (!IsHit)
		return;
	
	if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
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

		UAnimMontage* HitReactionMontage = GetMobData()->HitReactionMontage;
		if (HitReactionMontage)
		{
			AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
			IsHit = false;
		}
	}
}

void UHun_CombatComponent::PlayDeathAnimation()
{
	if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		FName SectionName = TEXT("Death_Front");

		if (HitAngle >= -90.0f && HitAngle <= 90.0f) 
			SectionName = TEXT("Death_Back");
		else
			SectionName = TEXT("Death_Front");
		
		UAnimMontage* DeathMontage = GetMobData()->DeathMontage;
		
		if (DeathMontage)
		{
			if (!DeathMontage->IsValidSectionName(SectionName))
			{
				SectionName = TEXT("Death");
			}
			AnimInstance->Montage_Play(DeathMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
		}
	}
}

void UHun_CombatComponent::CheckHitAngle(AActor* DamageCauser)
{
	if (CurrentHealthPoint > 0.0f && DamageCauser)
	{
		FVector HitDirection = (DamageCauser->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
		
		float ForwardDot = FVector::DotProduct(OwnerCharacter->GetActorLocation(), HitDirection);
		float RightDot = FVector::DotProduct(OwnerCharacter->GetActorLocation(), HitDirection);
		
		HitAngle = FMath::RadiansToDegrees(FMath::Atan2(RightDot, ForwardDot));

		HUN_LOG(FColor::Red, "HitAngle: %f", HitAngle);
	}
}

float UHun_CombatComponent::ApplyDamage(float TakenDamage)
{
	AActor* OwnerActor = GetOwner();
	
	if (CurrentHealthPoint <= 0.0f && !IsValid(OwnerActor))
		return 0.0f;

	float ActualDamage = TakenDamage; //여기서 방어도 스탯 공식 적용하면 될듯

	CurrentHealthPoint -= ActualDamage;
	
	if (OnHPChange.IsBound())
	{
		OnHPChange.Broadcast(CurrentHealthPoint, MaxHealthPoint);
	}
	
	return ActualDamage;
}

void UHun_CombatComponent::CharacterDie()
{
	PlayDeathAnimation();
	if (UCapsuleComponent* CapsuleComp = OwnerCharacter->GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh())
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	OwnerCharacter->SetLifeSpan(1.6f);
}



