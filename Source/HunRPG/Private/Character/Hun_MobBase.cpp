// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_MobBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HunRPG_DebugHelper.h"
#include "Interface/Hun_CombatInterface.h"


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

// Called every frame
void AHun_MobBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AHun_MobBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float InDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return IHun_CombatInterface::Execute_TakeDamage_interface(this, InDamage,DamageEvent, EventInstigator, DamageCauser);
}


