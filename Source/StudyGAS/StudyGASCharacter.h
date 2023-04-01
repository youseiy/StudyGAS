// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "StudyGASCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChange, float, AttributeValue);



/*Precisamos obrigatoriamente adicionar e implementar a interface IAbilitySystemInterface OBS: não esquecer do include: #include "AbilitySystemInterface.h"*/


enum class ECharacterAbilityInputID : uint8;
//Forward Declaration
class UCharacterAbilitySystemComponent;
class UCharacterAttributeSet;
class UCharacterGameplayAbility;
class UGameplayEffect;



UCLASS(config=Game)
class AStudyGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	//GAS
	//Declaração dos ponteiros para Criação.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TObjectPtr< UCharacterAttributeSet> AttributeSet;

	/*No GAS temos o conceito de que se queremos que o personagem  tenha algum GameplayEffect ou alguma Ability,
	 *essas precisam ser obtidas de alguma forma,seja no BeginPlay(Quando o jogo começar) ou quando o
	 *PlayerController(virtual void PossessedBy(AController* NewController) override;) for chamado
	 *vamos dizer quais habilidades nosso personagem vai ter e vamos atualizar nossos atributos para os valores que nessa caso seriam o padrão */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf< UCharacterGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> StartingDataTableAttribute;
	
	//...
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LightSpeedAction;

public:
	AStudyGASCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Funções GAS
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	void HandleLightSpeedPressed();
	void HandleLightSpeedRelesed();
	void SendInputToASC(bool bIsPressed, ECharacterAbilityInputID AbilityInputID);

	//Função de Ajuda para dar Update na UI
	UFUNCTION(BlueprintPure,Category=Attributes)
	void GetHealthValues(float &Health, float &MaxHealth) const;
	
};

