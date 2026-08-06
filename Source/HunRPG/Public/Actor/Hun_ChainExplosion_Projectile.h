#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Hun_ChainExplosion_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class HUNRPG_API AHun_ChainExplosion_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AHun_ChainExplosion_Projectile();
	void DesignateAtTarget(APlayerController* TargetCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|ChainExplosion")
	UParticleSystem* ChainExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|ChainExplosion")
	UParticleSystem* FinalExplosionParticle;
	UPROPERTY(VisibleAnywhere, Category="HunRPG|Target")
	APlayerController* HomingPC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|ChainExplosion")
	float Damage = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|ChainExplosion")
	float ChainExplosionRadius = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|ChainExplosion")
	float FinalExplosionRadius = 300.f;

	int32 MaxExplosionsCount = 0;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void ApplyDamage();

	FVector HitLocation;

	FTimerHandle EffectTimerHandle;
	FTimerHandle DamageTimerHandle;

	UFUNCTION()
	void PlayGroundEffect();
	UFUNCTION()
	void OnTargetReached(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};