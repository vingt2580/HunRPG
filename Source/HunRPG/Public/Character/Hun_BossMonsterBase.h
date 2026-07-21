// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_MonsterBase.h"
#include "Components/SphereComponent.h"
#include "Hun_BossMonsterBase.generated.h"

UCLASS()
class HUNRPG_API AHun_BossMonsterBase : public AHun_MonsterBase
{
	GENERATED_BODY()

public:
	AHun_BossMonsterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Boss")
	bool bIsCombat = false;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Boss")
	USphereComponent* DetectionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Boss")
	UAnimMontage* BossEnterAnimMontage;

	void PlayEnterAnimation();

	UFUNCTION()
	void OnDetectionOverlap(
		UPrimitiveComponent* OverLappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	float RecognitionRadius = 1500.f;
};
