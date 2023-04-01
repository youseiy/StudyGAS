// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"



//https://docs.unrealengine.com/5.0/en-US/gameplay-attributes-and-attribute-sets-for-the-gameplay-ability-system-in-unreal-engine/

//Macros que criam automaticamente Getters e Setters para seus atributos em C++
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



UCLASS()
class STUDYGAS_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	//Constructor
	UCharacterAttributeSet();
	
	//Declaração do attributo
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);
	

	
	/*O GAS não tem atualmente forma de limitar um atributo por
	maximo então criamos um novo attributo para dizer qual
	o maximo do Health nesse exemplo*/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);	

	/*Função que é chamada antes de qualquer mudança ao atributo,
	para o desenvolvedor ter total liberdade de fazer o que quiser. */
	//virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	
	/*Função que é chamada depois da mudança do attributo, Exemplo: Personagem leva dano e essa função poderia ter a logica para
	Limitar para que o dano não deixe a vida -100 por exemplo ou deixe +100 acima da vida maxima*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
};
