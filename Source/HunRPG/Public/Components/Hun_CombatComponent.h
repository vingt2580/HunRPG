// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hun_ActorComponent.h"
#include "Components/ActorComponent.h"
#include "HunRPG/Public/Interface/Hun_CombatInterface.h"
#include "HunRPG/Public/Components/Hun_StateComponent.h"
#include "Hun_CombatComponent.generated.h"

struct FHun_TraceLine
{
	FVector StartPoint;
	FVector EndPoint;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUNRPG_API UHun_CombatComponent : public UHun_ActorComponent, public IHun_CombatInterface
{
	GENERATED_BODY()

	UHun_CombatComponent();
	
	virtual void AttackInput_interface_Implementation() override;
	virtual void InitializeCombatData_Interface_Implementation(FHun_CombatValue CharacterCombatData) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	int32 CurrentComboCount = 0;  
	int32 MaxComboCount;
	float AttackMoveImpuls;
	float BaseDamage;
	
	bool IsAttacking = false;
	bool IsWeaponTrace =false;

	FVector WeaponStartPoint;
    FVector WeaponEndPoint;
	
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;
	
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<UHun_StateComponent> StateComponent;
	UPROPERTY()
	TObjectPtr<UAnimMontage> ComboMontage;
	
	bool CanAttack();

public:
	UFUNCTION(BlueprintCallable, Category="HunRPG|Combat")
	void StartComboAttack();

	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void HitAttack();
	
	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void OnSaveCombo();

	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void OnResetCombo();

	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void OnTrace_Attack();
	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void OffTrace_Attack();
	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void AttackWeaponTracing(USkeletalMeshComponent* MeshComponent);
};
