// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Hun_Tornado_Projectile.h"

#include "Character/Hun_Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AHun_Tornado_Projectile::AHun_Tornado_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->InitSphereRadius(120.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	RootComponent = CollisionComponent;

	TornadoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TornadoNiagara"));
	TornadoNiagara->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 800.f;
	ProjectileMovementComponent->MaxSpeed = 800.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bConstrainToPlane = true;
	ProjectileMovementComponent->Velocity = FVector(0.f, 0.f, GetActorLocation().Z);

	ProjectileMovementComponent->bIsHomingProjectile = false;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 4000.f;
}

void AHun_Tornado_Projectile::FireTornado(AActor* TargetPlayer, FVector Direction, bool bIsLeft)
{
	if (!IsValid(TargetPlayer))
		return;
	
	HomingTarget = TargetPlayer;

	float Angle = bIsLeft ? -60.f : 60.f;
	FVector LaunchDirection = Direction.RotateAngleAxis(Angle, FVector::UpVector);
	ProjectileMovementComponent->Velocity = LaunchDirection.GetSafeNormal() * ProjectileMovementComponent->InitialSpeed;
	
	ActiveHomingTimerDelegate.BindUObject(this, &AHun_Tornado_Projectile::ToggleHoming, true);
	InActiveHomingTimerDelegate.BindUObject(this, &AHun_Tornado_Projectile::ToggleHoming, false);
	
	GetWorldTimerManager().SetTimer(ActiveHomingTimerHandle, ActiveHomingTimerDelegate, 1.0f, false);
	GetWorldTimerManager().SetTimer(InActiveHomingTimerHandle, InActiveHomingTimerDelegate, 3.0f, false);
}

void AHun_Tornado_Projectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHun_Tornado_Projectile::OnTargetHit);
	SetLifeSpan(5.f);
}

void AHun_Tornado_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHun_Tornado_Projectile::ToggleHoming(bool bTooggle)
{
	if (!IsValid(HomingTarget))
		return;
	
	if (bTooggle)
	{
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
	}
	else
	{
		ProjectileMovementComponent->bIsHomingProjectile = false;
		ProjectileMovementComponent->HomingTargetComponent = NULL;
	}
	
}

void AHun_Tornado_Projectile::OnTargetHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != GetInstigator())
	{
		AHun_Character* HitCharacter = Cast<AHun_Character>(OtherActor);
		if (IsValid(HitCharacter))
		{
			UGameplayStatics::ApplyDamage(OtherActor, 40.f, GetInstigatorController(), this, UDamageType::StaticClass());
			HitCharacter->LaunchCharacter(FVector(0,0,1000.f), true, true);
			Destroy();
		}
	}
}

