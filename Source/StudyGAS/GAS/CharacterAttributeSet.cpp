// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"


UCharacterAttributeSet::UCharacterAttributeSet()
{
	
}


void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	//Chama implementação da class Pai
	Super::PostGameplayEffectExecute(Data);

	// Verifica se algum GameplayEffect afeta nosso attributo Health. 
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		/*Se estiver, Aplica mas restrige o valor para não ser negativo nem maior que MaxHealth.
		 Exemplo: Recebemos cura de 100 mas nossa vida está 95, logo fica com 100 e curamos só 5.
		 Outro caso seria para um dano não deixar nossa vida negativa*/
		
		SetHealth(FMath::Clamp<float>(GetHealth(), 0.0f,GetMaxHealth()));
	}
	
}


