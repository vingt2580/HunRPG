
#include "HunRPG/Public/Interface/Hun_CombatInterface.h"

#include "System/HunRPG_StateTypes.h"

void IHun_CombatInterface::AttackInput_interface_Implementation()
{
}

float IHun_CombatInterface::HunTakeDamage_interface_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return 0.f;
}

