#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hun_CombatInterface.generated.h"

UINTERFACE()
class UHun_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class HUNRPG_CORE_API IHun_CombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	void AttackInput_interface();

	/**
	 * Implementation
	 */

	virtual void AttackInput_interface_Implementation();
};
