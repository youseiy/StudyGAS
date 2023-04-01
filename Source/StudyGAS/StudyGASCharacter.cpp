// Copyright Epic Games, Inc. All Rights Reserved.

#include "StudyGASCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GAS/CharacterAbilitySystemComponent.h"
#include "GAS/CharacterAttributeSet.h"
#include "GAS/CharacterGameplayAbility.h"
#include "StudyGAS.h"
#include "Kismet/KismetSystemLibrary.h"







//////////////////////////////////////////////////////////////////////////
// AStudyGASCharacter

AStudyGASCharacter::AStudyGASCharacter()
{
	//Criação dos componentes do GAS
	AbilitySystemComponent=CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("Gameplay Ability Component"));
	//Quando criamos o AttributeSet ele é automaticamente ligado ao nosso AbilitySystemComponent
	AttributeSet=CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));







	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AStudyGASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	

	
}
//O GAS utiliza essa função para sempre ter refencia do nosso Ability System Component
UAbilitySystemComponent* AStudyGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AStudyGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	/*Quando o PlayerControler ter Controle do PlayerCharacter vamos inicializar os atributos com Datatable,
	 *dar as habilidades iniciais e informar ao ASC nossos parametros para podermos usarmos certas funcões*/
	if (AttributeSet && AbilitySystemComponent)
	{
			AbilitySystemComponent->InitAbilityActorInfo(this, this);
        	InitializeAttributes();
        	GiveAbilities();
        	AttributeSet->InitFromMetaDataTable(StartingDataTableAttribute);
		
	}

	
}

void AStudyGASCharacter::InitializeAttributes()
{
	//Se o Ability System Component e DefaultAttributeEffect forem válidos
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		//Inicializar os GameplayEffects
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			
		}
	}
}

void AStudyGASCharacter::GiveAbilities()
{
	
	if (AbilitySystemComponent)
	{
		// Concede ao player todas as habilidades do Tarray DefaultAbilities
		for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility.GetDefaultObject(), 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

//GAMEPLAY ABILITY SYSTEM NÃO SUPORTA ENHANCED INPUT SYSTEM DA UE5 NO MOMENTO APENAS INPUT ACTION, QUE NÃO É MAIS SUPORTADO,
//por isso precismos dizer quando uma tecla foi pressionada ou não, para utilizar o Enhanced Input System.

//Diz ao Ability System Component que pressionamos uma tecla que esta ligada a uma Habilidade
void AStudyGASCharacter::HandleLightSpeedPressed()
{
	SendInputToASC(true,ECharacterAbilityInputID::LightSpeed);
}
//Diz ao Ability System Component que paramos de uma tecla que esta ligada a uma Habilidade
void AStudyGASCharacter::HandleLightSpeedRelesed()
{
	SendInputToASC(false,ECharacterAbilityInputID::LightSpeed);
}

//AbilitySystemComponent recebe as informações e transmite pro ENUM e GameplayAbilty é ativada
void AStudyGASCharacter::SendInputToASC(bool bIsPressed, ECharacterAbilityInputID AbilityInputID)
{
	if (!AbilitySystemComponent)
	{
		UKismetSystemLibrary::PrintString(this,"Oh no!!!");
		return;
	}	
		

	if (bIsPressed)
	{
		UKismetSystemLibrary::PrintString(this,"EU SOU FLASSHHHH!!!");
		
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(AbilityInputID));
	}
	else
	{
		UKismetSystemLibrary::PrintString(this,"NÃO SOU O FLASHHHH! :c");
		
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(AbilityInputID));
	}
}



void AStudyGASCharacter::GetHealthValues( float &Health, float &MaxHealth) const
{
	//Pega os Valores atuais de Health e Maxhealth com buffs e debuffs. Se quiser valores sem buffs ou debuffs, utilizar GetBasevalue();
	Health=AttributeSet->GetHealthAttribute().GetGameplayAttributeData(AttributeSet)->GetCurrentValue();
	MaxHealth=AttributeSet->GetMaxHealthAttribute().GetGameplayAttributeData(AttributeSet)->GetCurrentValue();
}


//////////////////////////////////////////////////////////////////////////
// Input

void AStudyGASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStudyGASCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStudyGASCharacter::Look);
		
		//LightSpeed Ability
		EnhancedInputComponent->BindAction(LightSpeedAction, ETriggerEvent::Started, this, &AStudyGASCharacter::HandleLightSpeedPressed);
		EnhancedInputComponent->BindAction(LightSpeedAction, ETriggerEvent::Completed, this, &AStudyGASCharacter::HandleLightSpeedRelesed);
	}

}

void AStudyGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AStudyGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




