#include "Character/Hun_BossMonsterBase.h"
#include "HunRPG_DebugHelper.h"
#include "HunRPG/Public/System/HunCollisionChannels.h"


AHun_BossMonsterBase::AHun_BossMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->InitSphereRadius(RecognitionRadius);

	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);

	DetectionSphere->SetCollisionResponseToChannel(ECC_HunCharacter, ECR_Overlap);
}

void AHun_BossMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(DetectionSphere))
	{
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHun_BossMonsterBase::OnDetectionOverlap);
	}
}

void AHun_BossMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHun_BossMonsterBase::PlayEnterAnimation()
{
	if (bIsCombat)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
			return;
		
		AnimInstance->Montage_Play(BossEnterAnimMontage, 1.0f);
	}
}

void AHun_BossMonsterBase::OnDetectionOverlap(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (!bIsCombat)
		{
			bIsCombat = true;
			
			PlayEnterAnimation();
			
			HUN_LOG(FColor::Red, "보스구역 진입 진입 캐릭터 %s", *OtherActor->GetName());
			DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

