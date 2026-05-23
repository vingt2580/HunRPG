#include "Components/Hun_CombatComponent.h"

#include "HunRPG/Public/Components/Hun_CombatComponent.h"
#include "HunRPG/Public/System/HunRPG_StateTypes.h"
#include "HunRPG/Public/Components/Hun_StateComponent.h"
#include "HunRPG_DebugHelper.h"

#include "Animation/AnimInstance.h"
#include "Components/Hun_StateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/HunRPG_StateTypes.h"


UHun_CombatComponent::UHun_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHun_CombatComponent::AttackInput_interface_Implementation()
{
	if (!OwnerCharacter || !StateComponent)
		return;

	if (IsAttacking)
	{
		//HUN_LOG(FColor::Green, "Already Attacking");
		return;
	}

	StartComboAttack();
}

void UHun_CombatComponent::InitializeCombatData_Interface_Implementation(FHun_CombatValue CharacterCombatData)
{
	ComboMontage = CharacterCombatData.ComboMontage;
	MaxComboCount = CharacterCombatData.MaxComboCount;
	AttackMoveImpuls = CharacterCombatData.AttackMoveImpulse;
	BaseDamage = CharacterCombatData.BaseDamage;
}

void UHun_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	if (!OwnerActor)
		return;

	OwnerCharacter = Cast<ACharacter>(OwnerActor);

	if (!OwnerCharacter)
		return;
	
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

	if (!AnimInstance || !ComboMontage)
		return;
	
	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	OwnerCharacter->GetCharacterMovement()->AddImpulse(ForwardVector * AttackMoveImpuls, true);
	
	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);
	FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), CurrentComboCount));
	
	IsAttacking = true;
	StateComponent->SetState(EHunRPG_ActionState::Attacking);
	
	AnimInstance->Montage_Play(ComboMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, ComboMontage);
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
		float Alpha = static_cast<float>(i) / SegmentCount; //띠용

		const FVector PreviousMid = FMath::Lerp(WeaponStartPoint, WeaponStartPoint, Alpha);
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
				BaseDamage,
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

