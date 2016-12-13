// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "skycraft.h"
#include "skycraftGameMode.h"
#include "skycraftHUD.h"
#include "skycraftCharacter.h"

AskycraftGameMode::AskycraftGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AskycraftHUD::StaticClass();

	// Load point cloud
}
