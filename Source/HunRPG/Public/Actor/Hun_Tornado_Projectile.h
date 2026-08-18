// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Hun_Tornado_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class HUNRPG_API AHun_Tornado_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AHun_Tornado_Projectile();

	void FireTornado(AActor* TargetPlayer, FVector Direction, bool bIsLeft); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Tornado")
	UNiagaraComponent* TornadoNiagara;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UFUNCTION()
	void ActivateHoming();
	
	UFUNCTION()
	void OnTargetHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	FTimerHandle HomingTimer;

	UPROPERTY()
	AActor* HomingTarget;
};
