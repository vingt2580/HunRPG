// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Hun_MobBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HunRPG_DebugHelper.h"
#include "HunRPG/Public/Data/Hun_ComponentsData.h"
#include "Components/Hun_ActorComponent.h"
#include "Interface/Hun_CombatInterface.h"
#include "Interface/Hun_MovementInterface.h"

// Sets default values
AHun_MobBase::AHun_MobBase()
{
	PrimaryActorTick.bCanEverTick = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh();
	
	if (!SkeletalMeshComp)
		return;
	if (!CapsuleComp)
		return;
	
	CapsuleComp->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	SkeletalMeshComp->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
}

void AHun_MobBase::SetMobSpeed(float speed)
{
	if (!IsValid(CachedMovementComponent))
		return;
	
	IHun_MovementInterface::Execute_SetMoveSpeed_Interface(CachedMovementComponent, speed);
}

// Called when the game starts or when spawned
void AHun_MobBase::BeginPlay()
{
	Super::BeginPlay();
	
	UCharacterMovementComponent* MoveComponent = GetCharacterMovement();

	if (!MoveComponent)
		return;
	
	bUseControllerRotationYaw = false;
	
	MoveComponent->bOrientRotationToMovement = true; 
	MoveComponent->RotationRate = FRotator(0.f, 540.0f, 0.f);
}

float AHun_MobBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(CachedCombatComponent))
		return 0.f;
	
	float InDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return IHun_CombatInterface::Execute_HunTakeDamage_interface(CachedCombatComponent, InDamage,DamageEvent, EventInstigator, DamageCauser);
}

void AHun_MobBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!ComponentsData)
		return;

	for (TSubclassOf<UHun_ActorComponent> ComponentClass : ComponentsData->CharacterComponents)
	{
		if (ComponentClass)
		{
			UHun_ActorComponent* NewComponent = NewObject<UHun_ActorComponent>(this, ComponentClass);
			NewComponent->RegisterComponent();

			if (!IsValid(CachedMovementComponent) && NewComponent->Implements<UHun_MovementInterface>())
			{
				CachedMovementComponent = NewComponent;
			}

			if (!IsValid(CachedCombatComponent) && NewComponent->Implements<UHun_CombatInterface>())
			{
				CachedCombatComponent = NewComponent;
			}
		}
	}

	if (!IsValid(CachedMovementComponent))
		HUN_LOG(FColor::Red, "Failed to create and cache movement component");
	if (!IsValid(CachedMovementComponent))
		HUN_LOG(FColor::Red, "Failed to create and cache combat component");
}


