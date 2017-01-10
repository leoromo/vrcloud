// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SkyVR.h"
#include "SkyVRGameMode.h"
#include "SkyVRHUD.h"
#include "SkyVRCharacter.h"

ASkyVRGameMode::ASkyVRGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASkyVRHUD::StaticClass();
}
