// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hun_MonsterBase.h"
#include "Components/Hun_CombatComponent.h"
#include "Widget/Widget_HunMonsterHPBar.h"
#include "HunRPG/Public/System/HunCollisionChannels.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void AHun_MonsterBase::BeginPlay()
{
	Super::BeginPlay();

	BindStatusBar();
}

AHun_MonsterBase::AHun_MonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_HunMonster);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	
	Widget_MonsterHPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget_MonsterHPBar"));

	FVector2D HPBarDrawSize(800.f, 70.f);
	Widget_MonsterHPBar->SetupAttachment(GetRootComponent());
	Widget_MonsterHPBar->SetRelativeLocation(FVector(0.f,0.f,150.f));
	Widget_MonsterHPBar->SetRelativeScale3D(FVector(0.1f,0.1f,0.1f));
	
	Widget_MonsterHPBar->SetDrawSize(HPBarDrawSize);
	
}

void AHun_MonsterBase::BindStatusBar()
{
	if (!IsValid(Widget_MonsterHPBar))
		return;

	UWidget_HunMonsterHPBar* HPBar = Cast<UWidget_HunMonsterHPBar>(Widget_MonsterHPBar->GetUserWidgetObject());

	if (!HPBar)
		return;
	
	UHun_CombatComponent* CombatComp = FindComponentByClass<UHun_CombatComponent>();
	if (CombatComp)
	{
		HPBar->BindCombatComponent(CombatComp);
	}
}

void AHun_MonsterBase::OrientHpBar()
{
	if (!IsValid(Widget_MonsterHPBar))
		return;

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CameraManager)
	{
		FVector WidgetLocation = Widget_MonsterHPBar->GetComponentLocation();
		FVector CameraLocation = CameraManager->GetCameraLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

		Widget_MonsterHPBar->SetRelativeRotation(LookAtRotation);
	}
}

void AHun_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OrientHpBar();
}
