#pragma once

#include "CoreMinimal.h"
#include "Hun_Interface.h"
#include "UObject/Interface.h"
#include "Hun_CombatInterface.generated.h"
UENUM(BlueprintType)
enum class EHun_AbilityType : uint8
{
	None UMETA(DisplayName = "None"),
	Ability_A UMETA(DisplayName = "Ability_A"),
	Ability_B UMETA(DisplayName = "Ability_B"),
	Ultimate UMETA(DisplayName = "Ultimate"),
};
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
	void ActiveAbility_Interface(EHun_AbilityType AbilityType);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	float HunTakeDamage_interface(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser);

	/**
	 * Implementation	
	 */

	virtual void AttackInput_interface_Implementation();
	virtual void ActiveAbility_Interface_Implementation(EHun_AbilityType AbilityType);
	virtual float HunTakeDamage_interface_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) = 0;
};
