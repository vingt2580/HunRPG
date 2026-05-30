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
	AHun_MonsterAIController();

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

	UFUNCTION()
	void OnTarget(AActor* Actor, FAIStimulus Stimulus);
};
