// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


//Para Direcionarmos um input a uma Habilidade precisamos um ENUM
//que contanha obrigatoriament None,Confirm, Cancel e depois quantas Habilidades nosso Character possa ter.

UENUM(BlueprintType)
enum class ECharacterAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),

	Confirm			UMETA(DisplayName = "Confirm"),

	Cancel			UMETA(DisplayName = "Cancel"),
	
	Sprint			UMETA(DisplayName = "Sprint"),
	
	LightSpeed			UMETA(DisplayName = "LightSpeed")

	//Exemplo
	//Fire			UMETA(DisplayName = "Fire")

	//Reload			UMETA(DisplayName = "Reload")
};