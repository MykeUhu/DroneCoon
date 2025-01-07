// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneCoonGameMode.h"
#include "DroneCoonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADroneCoonGameMode::ADroneCoonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
