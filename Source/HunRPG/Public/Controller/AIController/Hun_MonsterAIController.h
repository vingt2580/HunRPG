// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Hun_MonsterAIController.generated.h"

UCLASS()
class HUNRPG_API AHun_MonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHun_MonsterAIController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|AI", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTarget(AActor* Actor, FAIStimulus Stimulus);
};
