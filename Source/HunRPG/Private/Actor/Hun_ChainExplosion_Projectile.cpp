#include "HunRPG/Public/Actor/Hun_ChainExplosion_Projectile.h"

#include "Character/Hun_Character.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/HunRPG_AbilityTypes.h"


AHun_ChainExplosion_Projectile::AHun_ChainExplosion_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->InitSphereRadius(40.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 800.f;
	ProjectileMovementComponent->MaxSpeed = 800.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 6000.f;

	ProjectileMovementComponent->bConstrainToPlane = true;
	ProjectileMovementComponent->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));
}

void AHun_ChainExplosion_Projectile::DesignateAtTarget(APlayerController* TargetPC)
{
	if (!IsValid(TargetPC) || !ProjectileMovementComponent)
		return;

	HomingPC = TargetPC;

	if (IsValid(TargetPC->GetPawn()))
	{
		ProjectileMovementComponent->HomingTargetComponent = TargetPC->GetPawn()->GetRootComponent();	
	}

	GetWorldTimerManager().SetTimer(EffectTimerHandle, this, &AHun_ChainExplosion_Projectile::PlayGroundEffect, 0.8f, true);
	
}

void AHun_ChainExplosion_Projectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this,&AHun_ChainExplosion_Projectile::OnTargetReached);
}

void AHun_ChainExplosion_Projectile::ApplyDamage()
{
	TArray<AActor*> IgnoreActors;
	if (IsValid(GetInstigator()))
	{
		IgnoreActors.Add(GetInstigator());
	}
	
	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		HitLocation,
		FinalExplosionRadius,
		UDamageType::StaticClass(),
		IgnoreActors,
		this,
		GetInstigatorController(),
		false);
	
	if (IsValid(HomingPC) && IsValid(HomingPC->GetPawn()))
	{
		AHun_Character* TargetCharacter = Cast<AHun_Character>(HomingPC->GetPawn());
		if (TargetCharacter)
		{
			FVector KnockbackDirection = FVector(0, 0, 1);
			TargetCharacter->LaunchCharacter(KnockbackDirection * 100.f, true, true);
		}
	}

	DrawDebugSphere(GetWorld(), HitLocation, FinalExplosionRadius, 12, FColor::Red, false, 2.0f);
	
	Destroy();
}

void AHun_ChainExplosion_Projectile::PlayGroundEffect()
{
	FVector GroundLocation = GetActorLocation();

	if (IsValid(HomingPC->GetPawn()))
	{
		ProjectileMovementComponent->HomingTargetComponent = HomingPC->GetPawn()->GetRootComponent();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ChainExplosionParticle, GroundLocation, FRotator::ZeroRotator, FVector(3.f));

	TArray<AActor*> IgnoreActors;

	if (!IsValid(GetInstigator()))
		return;

	IgnoreActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		GroundLocation,
		200.f,
		UDamageType::StaticClass(),
		IgnoreActors,
		this,
		GetInstigatorController(),
		false);
}

void AHun_ChainExplosion_Projectile::OnTargetReached(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == this)
		return;
	if (!IsValid(HomingPC) || !IsValid(HomingPC->GetPawn()))
		return;
    
	if (OtherActor == HomingPC->GetPawn())
	{
		GetWorldTimerManager().ClearTimer(EffectTimerHandle);

		HitLocation = GetActorLocation();
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FinalExplosionParticle, HitLocation, FRotator::ZeroRotator, FVector(3.f));
		
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->StopMovementImmediately();
		}
		if (CollisionComponent)
		{
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AHun_ChainExplosion_Projectile::ApplyDamage, 1.f, false);
	}
}