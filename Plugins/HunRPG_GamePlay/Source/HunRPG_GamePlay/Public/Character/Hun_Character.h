// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Data/Hun_CharacterData.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"

#include "Hun_Character.generated.h"

UCLASS()
class HUNRPG_GAMEPLAY_API AHun_Character : public ACharacter
{
	GENERATED_BODY()
	
	AHun_Character();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "HunRPG|Character Data")
	UHun_CharacterData* CharacterData;
	
	void Character_Move(FVector2D ActionValue);

	UPROPERTY()
	UActorComponent* CachedMovementComponent;

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion
};
