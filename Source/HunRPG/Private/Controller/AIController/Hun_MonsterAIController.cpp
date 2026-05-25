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

	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f; //AI 전용 데이터에셋 분리

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AHun_MonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIPerceptionComponent)
		return;

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AHun_MonsterAIController::OnTarget);
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

