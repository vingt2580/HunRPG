// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/Hun_MonsterAIController.h"

#include "HunRPG_DebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Hun_Character.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AHun_MonsterAIController::AHun_MonsterAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	UCrowdFollowingComponent* CrowdComp =Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AHun_MonsterAIController::OnTargetPerceptionUpdated);
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
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::AHun_MonsterAIController::OnTargetPerceptionUpdated);

	if (UCrowdFollowingComponent* CrowdComp =Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance? ECrowdSimulationState::Enabled: ECrowdSimulationState::Disabled);
		switch (DetourCrowdAvoidance)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
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

void AHun_MonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AHun_Character* TargetCharacter = Cast<AHun_Character>(Actor);
	if (!TargetCharacter)
		return;

	UBlackboardComponent* BBComponent = GetBlackboardComponent();
	if (!BBComponent)
		return;
	
	if (Stimulus.WasSuccessfullySensed() && Actor)
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

