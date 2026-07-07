// Fill out your copyright notice in the Description page of Project Settings.


#include "HunRPG/Public/GameMode/Hun_GameMode.h"

#include "Controller/PlayerController/Hun_PlayerController.h"

AHun_GameMode::AHun_GameMode()
{
	PlayerControllerClass = AHun_PlayerController::StaticClass();

	DefaultPawnClass = nullptr;
}
