// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define HUN_LOG(InColor, InFormat, ...) \
{ \
FString Msg = FString::Printf(TEXT(InFormat), ##__VA_ARGS__); \
if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, InColor, Msg); \
UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg); \
}