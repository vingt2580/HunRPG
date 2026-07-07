// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"

#include "System/Input/Hun_InputConfigData.h"
#include "Widget/Widget_HunHUD.h"
#include "Hun_PlayerController.generated.h"

/**
 * 
 */
class FinputActionValue;
class AHun_Character;

UCLASS()
class HUNRPG_API AHun_PlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AHun_PlayerController();
	virtual FGenericTeamId GetGenericTeamId() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunParty|Party")
	TArray<TSubclassOf<AHun_Character>> HunCaracterPartyClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunParty|Party")
	TArray<AHun_Character*> HunCharacterPartyMembers;

	int32 CurrentPartyMemberSlot = 0;

	UFUNCTION(BlueprintCallable, Category = "HunParty|Party")
	void SwapCharacter(int32 SlotIndex);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|InputData")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Controller")
	class UHun_InputConfigData* InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HunRPG|Character")
	class AHun_Character* HunCharacter;
	
public:
	void Input_Move(const FInputActionValue& ActionValue);
	void Input_Jump();
	void Input_Dash();
	void Input_CameraLook(const FInputActionValue& ActionValue);
	void Input_Attack();
	void Reset_MoveSpeed();

	void Input_Ablity_A();
	void Input_Ablity_B();
	void Input_Ultimate();

	void Input_SwapCharacter1();
	void Input_SwapCharacter2();
	void Input_SwapCharacter3();

private:
	FGenericTeamId TeamId;

	UPROPERTY(EditDefaultsOnly, Category = "HunRPG|UI")
	TSubclassOf<UWidget_HunHUD> MainHUDWidget;

	void AddtoViewportHUD();
	void SetupPartyMember();
	static void EjectionCharacter(AHun_Character* HunCharacter, bool Ejection);
};
