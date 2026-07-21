// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Hun_SequenceManager.h"

#include "MovieSceneSequencePlaybackSettings.h"
#include "MovieSceneSequencePlayer.h"
#include "Character/Hun_Character.h"
#include "HunRPG/Public/System/HunCollisionChannels.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"


AHun_SequenceManager::AHun_SequenceManager()
{
	SequenceTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SequenceTriggerBox"));
	RootComponent = SequenceTriggerBox;
	SequenceTriggerBox->SetBoxExtent(FVector(100, 100, 100));

	SequenceTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SequenceTriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	SequenceTriggerBox->SetCollisionResponseToChannel(ECC_HunCharacter, ECR_Overlap);
}

void AHun_SequenceManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SequenceTriggerBox))
	{
		SequenceTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHun_SequenceManager::OnOverlapTrigger);
	}
}

void AHun_SequenceManager::OnOverlapTrigger(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasPlay || !Sequence || !OtherActor->IsA(AHun_Character::StaticClass()))
		return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	
	if (!IsValid(PC))
		return;
	
	bHasPlay = true;

	PC->DisableInput(PC);

	ALevelSequenceActor* OutActor = nullptr;
	FMovieSceneSequencePlaybackSettings PlaybackSettings;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),Sequence, PlaybackSettings, OutActor);

	if (IsValid(SequencePlayer))
	{
		SequencePlayer->OnFinished.AddDynamic(this,&AHun_SequenceManager::OnSequenceFinished);
		SequencePlayer->Play();
	}
}

void AHun_SequenceManager::OnSequenceFinished()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	
	if (!IsValid(PC))
		return;

	PC->EnableInput(PC);
}


