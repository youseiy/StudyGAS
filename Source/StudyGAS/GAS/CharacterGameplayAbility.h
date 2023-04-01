// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "StudyGAS/StudyGAS.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class STUDYGAS_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCharacterGameplayAbility();
	//Criação do ENUM para termos a referencia do GameplayAbility sobre qual habilidade e qual input vamos ter
	UPROPERTY(EditAnywhere,Category=AbilityID)
	ECharacterAbilityInputID AbilityInputID =ECharacterAbilityInputID::None;
	
	
	
};
