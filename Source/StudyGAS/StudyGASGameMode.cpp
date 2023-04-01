// Copyright Epic Games, Inc. All Rights Reserved.

#include "StudyGASGameMode.h"
#include "StudyGASCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStudyGASGameMode::AStudyGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
