#include "Components/Hun_ActorComponent.h"
#include "GameFramework/Character.h"
#include "Character/Hun_MobBase.h"
#include "Data/Hun_CharacterData.h"

UHun_ActorComponent::UHun_ActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHun_ActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<AHun_MobBase>(GetOwner());
	
	if (!MobData)
		return;
	
	InitializeMobData();
}

void UHun_ActorComponent::SetData(UHun_CharacterData* NewHunData)
{
	if (!IsValid(MobData))
		return;
	if (!NewHunData)
		return;

	MobData = NewHunData;
	InitializeMobData();
}

void UHun_ActorComponent::InitializeMobData()
{
}

