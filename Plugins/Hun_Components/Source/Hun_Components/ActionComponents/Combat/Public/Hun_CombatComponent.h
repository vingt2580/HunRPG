// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HunRPG_Core/Public/Interface/Hun_CombatInterface.h"
#include "Hun_CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUN_COMPONENTS_API UHun_CombatComponent : public UActorComponent, public IHun_CombatInterface
{
	GENERATED_BODY()

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
	
	bool IsAttacking = false;
	
	UPROPERTY()
	ACharacter* OwnerCharacter;
	UPROPERTY()
	class UHun_StateComponent* StateComponent;
	
	UPROPERTY()
	class UAnimMontage* ComboMontage;

	bool CanAttack();

public:
	UFUNCTION(BlueprintCallable, Category="HunRPG|Combat")
	void StartComboAttack();
	
	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void OnSaveCombo();

	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	void OnResetCombo();
	
};
