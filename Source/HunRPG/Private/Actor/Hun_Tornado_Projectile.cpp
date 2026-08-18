// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Hun_Tornado_Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AHun_Tornado_Projectile::AHun_Tornado_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->InitSphereRadius(120.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	TornadoNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TornadoNiagara"));
	TornadoNiagara->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 800.f;
	ProjectileMovementComponent->MaxSpeed = 800.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

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
	ProjectileMovementComponent->Velocity = LaunchDirection * ProjectileMovementComponent->InitialSpeed;
	GetWorldTimerManager().SetTimer(HomingTimer, this, &AHun_Tornado_Projectile::ActivateHoming, 1.0f, false);
}

void AHun_Tornado_Projectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHun_Tornado_Projectile::OnTargetHit);
}

void AHun_Tornado_Projectile::ActivateHoming()
{
	if (!IsValid(HomingTarget))
		return;

	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
}

void AHun_Tornado_Projectile::OnTargetHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != GetInstigator())
	{
		UGameplayStatics::ApplyDamage(OtherActor, 40.f, GetInstigatorController(), this, UDamageType::StaticClass());

		ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
		if (IsValid(HitCharacter))
		{
			HitCharacter->LaunchCharacter(FVector(0,0,1000.f), true, true);
		}

		Destroy();
	}
}

