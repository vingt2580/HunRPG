// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/PlayerController/Hun_PlayerController.h"
#include "Character/Hun_Character.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HunRPG_DebugHelper.h"
#include "InputAction.h"
#include "Components/Hun_CombatComponent.h"
#include "Components/Hun_MoveComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/Hun_CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Widget_HunPlayerHPBar.h"
#include "Widget/WIdget_HunStaminaGauge.h"

AHun_PlayerController::AHun_PlayerController()
{
	TeamId = FGenericTeamId(0);
}

FGenericTeamId AHun_PlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void AHun_PlayerController::SwapCharacter(const int32 SlotIndex)
{
	if (!HunCharacterPartyMembers.IsValidIndex(SlotIndex) || SlotIndex == CurrentPartyMemberSlot)
		return;
	
	AHun_Character* PrevCharacter = HunCharacterPartyMembers[CurrentPartyMemberSlot];
	AHun_Character* NextCharacter = HunCharacterPartyMembers[SlotIndex];

	if (!IsValid(PrevCharacter)) HUN_LOG(FColor::Red, "PrevCharacter가 비어있음 이전 슬롯 : %d", CurrentPartyMemberSlot);
	if (!IsValid(NextCharacter)) HUN_LOG(FColor::Red, "NextCharacter가 비어있음 불러올 슬롯 : %d", SlotIndex);

	if (IsValid(PrevCharacter) && IsValid(NextCharacter))
	{
		bool bLockedOn = PrevCharacter->IsLockedOn();
		AActor* SaveLockOnTarget = PrevCharacter->GetLockOnTarget();

		PrevCharacter->ClearLockOnTarget();
		
		const FVector PrevLocation = PrevCharacter->GetActorLocation();
		const FRotator PrevRotation = PrevCharacter->GetActorRotation();

		const FRotator PrevControlRotation = PrevCharacter->GetControlRotation();

		float PrevCameraDistance = PrevCharacter->GetCameraBoom()->TargetArmLength; 

		EjectionCharacter(PrevCharacter, true);

		NextCharacter->SetActorLocationAndRotation(PrevLocation, PrevRotation);
		EjectionCharacter(NextCharacter, false);

		Possess(NextCharacter);
		HunCharacter = NextCharacter;
		
		SetViewTargetWithBlend(NextCharacter);
		SetControlRotation(PrevControlRotation);
		NextCharacter->GetCameraBoom()->TargetArmLength = PrevCameraDistance;

		if (bLockedOn && IsValid(SaveLockOnTarget))
		{
			NextCharacter->ForceSetLockOnTarget(SaveLockOnTarget);
		}

		UpdateWidgetBinding(PrevCharacter, NextCharacter);
		CurrentPartyMemberSlot = SlotIndex;

		HUN_LOG(FColor::Blue, "캐릭터 교체 성공 현재 캐릭터 슬록 %d, 현재 조종 캐릭터이름 : %s", CurrentPartyMemberSlot, *NextCharacter->GetName());
	}
}

void AHun_PlayerController::UpdateWidgetBinding(const AHun_Character* PrevCharacter, const AHun_Character* TargetCharacter) const
{
	if (IsValid(MainHUD) == false)
	{
		return;
	}

	if (IsValid(PrevCharacter))
	{
		UHun_CombatComponent* CombatComponent = PrevCharacter->FindComponentByClass<UHun_CombatComponent>();
		UHun_MoveComponent* MoveComponent = PrevCharacter->FindComponentByClass<UHun_MoveComponent>();

		if (IsValid(CombatComponent) && MainHUD->PlayerStatusWidget)
		{
			MainHUD->PlayerStatusWidget->UnbindCombatComponent(CombatComponent);
			HUN_LOG(FColor::Blue, "체력 UI 언바인딩 완료");
		}

		if (IsValid(MoveComponent) && MainHUD->StaminaGauge)
		{
			MainHUD->StaminaGauge->UnbindMoveComponent(MoveComponent);
			HUN_LOG(FColor::Blue, "스테미나 UI 언바인딩 완료");
		}
	}
	
	if (IsValid(TargetCharacter))
	{
		UHun_CombatComponent* CombatComponent = TargetCharacter->FindComponentByClass<UHun_CombatComponent>();
		UHun_MoveComponent* MoveComponent = TargetCharacter->FindComponentByClass<UHun_MoveComponent>();

		if (IsValid(CombatComponent) && MainHUD->PlayerStatusWidget)
		{
			MainHUD->PlayerStatusWidget->BindCombatComponent(CombatComponent);
			HUN_LOG(FColor::Blue, "체력 UI 바인딩 완료");
		}

		if (IsValid(MoveComponent) && MainHUD->StaminaGauge)
		{
			MainHUD->StaminaGauge->BindMoveComponent(MoveComponent);
			HUN_LOG(FColor::Blue, "스테미나 UI 바인딩 완료");
		}
	}
}

void AHun_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetupPartyMember();
}

void AHun_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHun_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//HunCharacter = GetPawn<AHun_Character>();

	//if (!IsValid(HunCharacter))
		//return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(PEI))
		return;
	
	if (IsValid(InputActions))
	{
		PEI->BindAction(InputActions->Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		PEI->BindAction(InputActions->Input_Move, ETriggerEvent::Completed, this, &ThisClass::Reset_MoveSpeed);
		PEI->BindAction(InputActions->Input_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
		PEI->BindAction(InputActions->Input_Dash, ETriggerEvent::Started, this, &ThisClass::Input_Dash);
		PEI->BindAction(InputActions->Input_Look, ETriggerEvent::Triggered ,this, &ThisClass::Input_CameraLook);
		PEI->BindAction(InputActions->Input_Attack, ETriggerEvent::Started, this, &ThisClass::Input_Attack);
		PEI->BindAction(InputActions->Input_LockOn, ETriggerEvent::Started, this, &ThisClass::Input_Lockon);
		PEI->BindAction(InputActions->Input_Ablity_A, ETriggerEvent::Started, this, &ThisClass::Input_Ablity_A);
		PEI->BindAction(InputActions->Input_Ablity_B, ETriggerEvent::Started, this, &ThisClass::Input_Ablity_B);
		PEI->BindAction(InputActions->Input_Ultimate, ETriggerEvent::Started, this, &ThisClass::Input_Ultimate);
		
		PEI->BindAction(InputActions->Input_Swap1, ETriggerEvent::Started, this, &ThisClass::Input_SwapCharacter1);
		PEI->BindAction(InputActions->Input_Swap2, ETriggerEvent::Started, this, &ThisClass::Input_SwapCharacter2);
		PEI->BindAction(InputActions->Input_Swap3, ETriggerEvent::Started, this, &ThisClass::Input_SwapCharacter3);
	}
}

void AHun_PlayerController::Input_Move(const FInputActionValue& ActionValue)
{
	if (!IsValid(HunCharacter))
		return;
	
	FVector2D MovementVector = ActionValue.Get<FVector2D>();
	HunCharacter->Character_Move(MovementVector);
}

void AHun_PlayerController::Input_Jump()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Jump();
}

void AHun_PlayerController::Input_Dash()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Dash();
}

void AHun_PlayerController::Input_CameraLook(const FInputActionValue& ActionValue)
{
	if (!IsValid(HunCharacter))
		return;
	
	FVector2D LookAxisValue = ActionValue.Get<FVector2D>();
	HunCharacter->Character_Look(LookAxisValue);
}

void AHun_PlayerController::Input_Attack()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Attack();
}

void AHun_PlayerController::Input_Lockon()
{
	if (!IsValid(HunCharacter))
		return;

	HunCharacter->ToggleLockOn();
}

void AHun_PlayerController::Reset_MoveSpeed()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_ResetMove();
}

void AHun_PlayerController::Input_Ablity_A()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Ability(EHun_AbilityType::Ability_A);
}

void AHun_PlayerController::Input_Ablity_B()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Ability(EHun_AbilityType::Ability_B);
}

void AHun_PlayerController::Input_Ultimate()
{
	if (!IsValid(HunCharacter))
		return;
	
	HunCharacter->Character_Ability(EHun_AbilityType::Ultimate);
}

void AHun_PlayerController::Input_SwapCharacter1()
{
	if (!IsValid(HunCharacter))
		return;

	SwapCharacter(0);
}

void AHun_PlayerController::Input_SwapCharacter2()
{
	if (!IsValid(HunCharacter))
		return;

	SwapCharacter(1);
}

void AHun_PlayerController::Input_SwapCharacter3()
{
	if (!IsValid(HunCharacter))
		return;

	SwapCharacter(2);
}

void AHun_PlayerController::SetupPartyMember()
{
	FVector CharacterSpawnLocation = FVector::ZeroVector;
	FRotator CharacterSpawnRotation = FRotator::ZeroRotator;

	if (AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()))
	{
		CharacterSpawnLocation = PlayerStart->GetActorLocation();
		CharacterSpawnRotation = PlayerStart->GetActorRotation();
	}

	AHun_Character* FirstMember = nullptr;
	
	for (int32 i = 0; i < HunCaracterPartyClasses.Num(); i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AHun_Character* SpawnedCharacter = GetWorld()->SpawnActor<AHun_Character>(
			HunCaracterPartyClasses[i],
			CharacterSpawnLocation,
			CharacterSpawnRotation,
			SpawnParams);

		if (IsValid(SpawnedCharacter))
		{
			int32 AddedIndex = HunCharacterPartyMembers.Add(SpawnedCharacter);

			HUN_LOG(FColor::Red, "HunCharacterPartyMembers[%d] : %s", AddedIndex, *HunCharacterPartyMembers[AddedIndex]->GetName());

			if (i == 0)
			{
				FirstMember = SpawnedCharacter;
				Possess(FirstMember);
				SetViewTargetWithBlend(FirstMember);
				HunCharacter = SpawnedCharacter;
				CurrentPartyMemberSlot = 0;
			}
			else
			{
				EjectionCharacter(SpawnedCharacter, true);
			}
		}
	}

	for (int32 i = 0; i < HunCharacterPartyMembers.Num(); i++)
	{
		//HUN_LOG(FColor::Red, "HunCharacterPartyMembers[%d] : %s", i, *HunCharacterPartyMembers[i]->GetName());
	}

	if (IsValid(Widget_HunHUD))
	{
		MainHUD = CreateWidget<UWidget_HunHUD>(GetWorld(), Widget_HunHUD);
		if (IsValid(MainHUD))
		{
			MainHUD->AddToViewport();
			UpdateWidgetBinding(nullptr,FirstMember);
		}
	}
}

void AHun_PlayerController::EjectionCharacter(AHun_Character* InHunCharacter, bool Ejection)
{
	if (Ejection)
	{
		InHunCharacter->SetActorHiddenInGame(true);
		InHunCharacter->SetActorEnableCollision(false);
		InHunCharacter->SetActorTickEnabled(false);
		InHunCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		InHunCharacter->SetActorHiddenInGame(false);
		InHunCharacter->SetActorEnableCollision(true);
		InHunCharacter->SetActorTickEnabled(true);
		InHunCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}
