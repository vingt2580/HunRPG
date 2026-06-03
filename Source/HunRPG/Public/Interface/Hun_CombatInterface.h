#pragma once

#include "CoreMinimal.h"
#include "Hun_Interface.h"
#include "UObject/Interface.h"
#include "Hun_CombatInterface.generated.h"

UINTERFACE()
class UHun_CombatInterface : public UHun_Interface
{
	GENERATED_BODY()
};

class HUNRPG_API IHun_CombatInterface : public IHun_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void AttackInput_interface();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	float HunTakeDamage_interface(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser);

	/**
	 * Implementation	
	 */

	virtual void AttackInput_interface_Implementation();
	virtual float HunTakeDamage_interface_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) = 0;
};
