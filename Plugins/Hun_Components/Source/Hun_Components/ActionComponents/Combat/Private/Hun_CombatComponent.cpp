#include "Hun_Components/ActionComponents/Combat/Public/Hun_CombatComponent.h"
#include "HunRPG_Core/Public/System/HunRPG_StateTypes.h"

#include "HunRPG_DebugHelper.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Hun_Components/ActionComponents/State/Public/Hun_StateComponent.h"


void UHun_CombatComponent::AttackInput_interface_Implementation()
{
	if (!OwnerCharacter || !StateComponent)
		return;

	if (IsAttacking)
	{
		HUN_LOG(FColor::Green, "Already Attacking");
		return;
	}

	StartComboAttack();
}

void UHun_CombatComponent::InitializeCombatData_Interface_Implementation(FHun_CombatValue CharacterCombatData)
{
	ComboMontage = CharacterCombatData.ComboMontage;
	MaxComboCount = CharacterCombatData.MaxComboCount;
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

	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);
	FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), CurrentComboCount));
	
	IsAttacking = true;
	StateComponent->SetState(EHunRPG_ActionState::Attacking);
	
	AnimInstance->Montage_Play(ComboMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, ComboMontage);
}

void UHun_CombatComponent::OnSaveCombo()
{
	IsAttacking = false;
	HUN_LOG(FColor::Green, "End Attacking");
}

void UHun_CombatComponent::OnResetCombo()
{
	if (!OwnerCharacter)
		return;
	
	IsAttacking = false;
	CurrentComboCount = 0;

	StateComponent->SetState(EHunRPG_ActionState::Idle);

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && ComboMontage)
	{
		AnimInstance->Montage_Stop(0.2f, ComboMontage);
	}
}

