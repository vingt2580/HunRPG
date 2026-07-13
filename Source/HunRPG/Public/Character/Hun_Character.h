// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hun_MobBase.h"
#include "Components/Hun_ActorComponent.h"
#include "Data/Hun_ComponentsData.h"

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"

#include "Hun_Character.generated.h"

enum class EHun_AbilityType : uint8;

UCLASS()
class HUNRPG_API UHunTestClass : public UObject
{
	GENERATED_BODY()

public:
	int A = 10;
};

UCLASS()
class HUNRPG_API AHun_Character : public AHun_MobBase
{
	GENERATED_BODY()
	
	AHun_Character();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	AActor* FindLockOnTarget() const;

	void DeadZoneLockOn(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|LockOn")
	bool bIsLockedOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HunRPG|LockOn")
	TObjectPtr<AActor> CurrentLockOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|LockOn")
	float LockOnRadius = 1500.f;

	float DeadZoneYaw = 15.f;
	float DeadzonePitch = 10.f;

public:
	
	void Character_Move(FVector2D ActionValue);
	void Character_ResetMove();
	void Character_Jump();
	void Character_Dash();
	void Character_Look(FVector2D LookAxisVector);
	void Character_Attack();
	void Character_Ability(EHun_AbilityType AbilityType);

	void ToggleLockOn();

	UFUNCTION()
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion
};
