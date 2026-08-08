#include "Character/Hun_BossMonsterBase.h"
#include "HunRPG_DebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/AIController/Hun_BossMonsterAIController.h"
#include "Controller/PlayerController/Hun_PlayerController.h"
#include "HunRPG/Public/System/HunCollisionChannels.h"
#include "Interface/Hun_CombatInterface.h"
#include "Kismet/GameplayStatics.h"


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

void AHun_BossMonsterBase::ExecuteChangePhase(int32 Phase)
{
	AHun_BossMonsterAIController* AIController = Cast<AHun_BossMonsterAIController>(GetController());
	
	if (!IsValid(AIController))
		return;
	
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

	if (!IsValid(BlackboardComp))
		return;

	BlackboardComp->SetValueAsInt(FName("Phase"), Phase);
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

			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			if (!IsValid(PC))
				return;
			AHun_PlayerController* HunPC = Cast<AHun_PlayerController>(PC);
			if (!HunPC)
				return;

			if (APawn* TargetPawn = Cast<APawn>(OtherActor))
			{
				CombatTargetPC = Cast<AHun_PlayerController>(TargetPawn->GetController());
				
				if (IsValid(CombatTargetPC))
				{
					HunPC->UpdateWidgetMonster(this);
			
					PlayEnterAnimation();
			
					HUN_LOG(FColor::Red, "보스구역 진입 진입 캐릭터 %s", *OtherActor->GetName());
					DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					AHun_BossMonsterAIController* BossAIPC = Cast<AHun_BossMonsterAIController>(GetController());
					if (IsValid(BossAIPC))
						BossAIPC->StartBossCombat(CombatTargetPC->GetPawn());

					OnBossCombat.Broadcast(bIsCombat);
				}
			}
		}
	}
}

