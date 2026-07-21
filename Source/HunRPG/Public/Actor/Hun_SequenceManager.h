// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hun_SequenceManager.generated.h"

class UBoxComponent;
class ULevelSequence;
class ULevelSequencePlayer;

UCLASS()
class HUNRPG_API AHun_SequenceManager : public AActor
{
	GENERATED_BODY()

	AHun_SequenceManager();

	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Sequence")
	UBoxComponent* SequenceTriggerBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Sequence")
	ULevelSequence* Sequence;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Sequence")
	ULevelSequencePlayer* SequencePlayer = nullptr;
	
private:
	UFUNCTION()
	void OnOverlapTrigger(UPrimitiveComponent* OverLappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSequenceFinished();

	bool bHasPlay = false;
};
