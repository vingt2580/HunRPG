// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_BossMonsterBase.h"
#include "Hun_Grux.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGruxLeapEndDelegate);

UCLASS()
class HUNRPG_API AHun_Grux : public AHun_BossMonsterBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Category="HunRPG|Pattern")
	void ExecuteLeapAttack_BossPattern();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FOnGruxLeapEndDelegate OnGruxLeapEnd;

	UPROPERTY()
	bool bIsLeaping = false;

private:
	virtual void Landed(const FHitResult& Hit) override;
};
