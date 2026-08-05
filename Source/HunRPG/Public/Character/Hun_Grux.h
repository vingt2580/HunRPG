// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_BossMonsterBase.h"
#include "HunRPG/Public/Actor/Hun_ChainExplosion_Projectile.h"
#include "Hun_Grux.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGruxLeapEndDelegate);

UCLASS()
class HUNRPG_API AHun_Grux : public AHun_BossMonsterBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Category="HunRPG|Pattern")
	void ExecuteLeapAttack_BossPattern();
	UFUNCTION(Category="HunRPG|Pattern")
	void ExecuteLandingShockwave();
	UFUNCTION(Category="HunRPG|Pattern")
	void ExecuteChainExplosion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HunRPG|ChainExplosion")
	TSubclassOf<class AHun_ChainExplosion_Projectile> ChainExplosionClass;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FOnGruxLeapEndDelegate OnGruxLeapEnd;

	UPROPERTY()
	bool bIsLeaping = false;

private:
	virtual void Landed(const FHitResult& Hit) override;

#pragma region SkillValue //여긴 추후 리펙토링때 데이터에셋으로 분리할 예정
	/**
	 * Shockwave
	 */
	float ShockwaveRadius = 800.0f;
	float ShockwaveDamage = 20.0f;
	float KnockbackPower = 1500.0f;
#pragma endregion
};
