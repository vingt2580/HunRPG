// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_MonsterBase.h"


// Sets default values
AHun_MonsterBase::AHun_MonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHun_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHun_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHun_MonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

