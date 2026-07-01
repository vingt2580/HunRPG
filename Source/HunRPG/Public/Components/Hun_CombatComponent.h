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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChangeDelegate, float, CurrentHP, float, MaxHP);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HUNRPG_API UHun_CombatComponent : public UHun_ActorComponent, public IHun_CombatInterface
{
	GENERATED_BODY()

	UHun_CombatComponent();
	
public:
	virtual void AttackInput_interface_Implementation() override;
	virtual float HunTakeDamage_interface_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ActiveAbility_Interface_Implementation(EHun_AbilityType AbilityType) override;

	UPROPERTY(BlueprintAssignable)
	FOnHPChangeDelegate OnHPChange;

	UPROPERTY()
	float CurrentHealthPoint;
	
	float MaxHealthPoint;
	
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
	TObjectPtr<UHun_StateComponent> StateComponent;
	UPROPERTY()
	TObjectPtr<UAnimMontage> ComboMontage;
	
	bool CanAttack();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|Combat")
	float HitAngle;
	UPROPERTY(BlueprintReadWrite, Category = "HunRPG|Combat")
	bool IsHit;
	
	
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
	UFUNCTION(BlueprintCallable, Category = "HunRPG|Combat")
	bool IsAlive() const { return CurrentHealthPoint > 0.0f; }
	
	void PlayHitAnimation();
	void PlayDeathAnimation();
	void CheckHitAngle(AActor* DamageCauser);
	float ApplyDamage(float TakenDamage);
	void CharacterDie();
};
