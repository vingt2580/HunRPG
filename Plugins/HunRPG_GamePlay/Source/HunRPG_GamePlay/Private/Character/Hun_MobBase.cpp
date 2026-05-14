// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_MobBase.h"

#include "HunRPG_DebugHelper.h"


// Sets default values
AHun_MobBase::AHun_MobBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CurrentHealthPoint = MobData->MaxHealthPoint;
}

// Called when the game starts or when spawned
void AHun_MobBase::BeginPlay()
{
	Super::BeginPlay();
	
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

	if (CurrentHealthPoint <= 0.0f)
		return 0.0f;

	CurrentHealthPoint -= TakenDamage;
	HUN_LOG(FColor::Magenta, "TakeDamage: %f", TakenDamage);
	
	return TakenDamage;
}

void AHun_MobBase::Die()
{
	HUN_LOG(FColor::Red, "MobBase사망");
}

