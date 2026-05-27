// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/Hun_MonsterAIController.h"

#include "HunRPG_DebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Hun_Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AHun_MonsterAIController::AHun_MonsterAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AHun_MonsterAIController::OnTarget);
}

void AHun_MonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(AIPerceptionComponent)||
		!IsValid(SightConfig)||
		!IsValid(CharacterData))
		return;

	SightConfig->SightRadius = CharacterData->AIValue.SightRadius;
	SightConfig->LoseSightRadius = CharacterData->AIValue.LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = CharacterData->AIValue.PeripheralVisionAngleDegrees;
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AHun_MonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!BehaviorTree)
		return;
	
	RunBehaviorTree(BehaviorTree);

	if (UBlackboardComponent* BBComponent = GetBlackboardComponent())
	{
		BBComponent->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
	}
}

void AHun_MonsterAIController::OnTarget(AActor* Actor, FAIStimulus Stimulus)
{
	AHun_Character* TargetCharacter = Cast<AHun_Character>(Actor);
	if (!TargetCharacter)
		return;

	UBlackboardComponent* BBComponent = GetBlackboardComponent();
	if (!BBComponent)
		return;

	if (Stimulus.WasSuccessfullySensed())
	{
		HUN_LOG(FColor::Red, "AIController Found!");
		BBComponent->SetValueAsObject(TEXT("TargetActor"), TargetCharacter);
	}
	else
	{
		HUN_LOG(FColor::Red, "AIController Not Found!");
		BBComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
	}
}

