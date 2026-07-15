// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Data/Hun_CharacterData.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Hun_MonsterAIController.generated.h"

UCLASS()
class HUNRPG_API AHun_MonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHun_MonsterAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "HunRPG|AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "HunRPG|AI")
	TObjectPtr<UHun_CharacterData> CharacterData;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "HunRPG|AI")
	bool bEnableDetourCrowdAvoidance;
	UPROPERTY(EditDefaultsOnly, Category = "HunRPG|AI", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UImin = "1", UMax = "4"))
	int32 DetourCrowdAvoidance = 4;
	UPROPERTY(EditDefaultsOnly, Category = "HunRPG|AI", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;
	
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
