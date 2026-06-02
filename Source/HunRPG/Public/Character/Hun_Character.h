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

UCLASS()
class HUNRPG_API AHun_Character : public AHun_MobBase
{
	GENERATED_BODY()
	
	AHun_Character();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HunRPG | Components")
	TObjectPtr<UHun_ComponentsData> ComponentsData;

public:
	
	void Character_Move(FVector2D ActionValue);
	void Character_ResetMove();
	void Character_Jump();
	void Character_Dash();
	void Character_Look(FVector2D LookAxisVector);
	void CHaracter_Attack();
	
	UPROPERTY()
	TObjectPtr<UHun_ActorComponent> CachedMovementComponent;
	UPROPERTY()
	TObjectPtr<UHun_ActorComponent> CachedCombatComponent;

	UPROPERTY()
	TArray<UHun_ActorComponent*> Component;

private:

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion
};
