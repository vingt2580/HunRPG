#pragma once

#include "CoreMinimal.h"
#include "HunRPG_AbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EHun_AbilityShape : uint8
{
	Circle UMETA(DisplayName = "원형"),
	Box UMETA(DisplayName = "박스"),
	Cone UMETA(DisplayName = "부채꼴")
};

UENUM(BlueprintType)
enum class EHun_AbilityEffect : uint8
{
	None UMETA(DisplayName = "물리"),
	Stun UMETA(DisplayName = "기절"),
	Burn UMETA(DisplayName = "화상")
};

USTRUCT(BlueprintType)
struct FHun_AbilityInfo
{
	GENERATED_BODY()

	//범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Range")
	EHun_AbilityShape Shape = EHun_AbilityShape::Circle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Range")
	float Radius = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Range")
	FVector BoxExtend = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Range")
	float ConeAngle = 0.0f;

	//전투
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Combat")
	float AbilityBaseDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Combat")
	int32 MaxTargets = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG|Combat")
	 TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Combat")
	TArray<EHun_AbilityEffect> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HunRPG | Combat")
	float KnockbackForce = 0.0f;
};
