#include "Hun_Components/ActionComponents/Combat/Public/Hun_CombatComponent.h"

#include "HunRPG_DebugHelper.h"


void UHun_CombatComponent::AttackInput_interface_Implementation()
{
	HUN_LOG(FColor::Red, "Input Attack");
}

void UHun_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UHun_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

