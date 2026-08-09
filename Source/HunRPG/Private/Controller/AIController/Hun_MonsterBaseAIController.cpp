// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/Hun_MonsterBaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/PlayerController/Hun_PlayerController.h"
#include "Kismet/GameplayStatics.h"

AHun_MonsterBaseAIController::AHun_MonsterBaseAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHun_MonsterBaseAIController::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		AHun_PlayerController* HunPC = Cast<AHun_PlayerController>(PC);
		if (IsValid(HunPC))
			HunPC->OnSwapTargetPlayer.AddUObject(this, &AHun_MonsterBaseAIController::SwapTargetPlayer);
	}
}

void AHun_MonsterBaseAIController::SwapTargetPlayer(AHun_Character* TargetPlayer)
{
	if (bIsGetTarget)
	{
		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

		if (!IsValid(BlackboardComponent))
			return;
		
		BlackboardComponent->SetValueAsObject(FName("TargetPlayer"), TargetPlayer);	
		
	}
}

