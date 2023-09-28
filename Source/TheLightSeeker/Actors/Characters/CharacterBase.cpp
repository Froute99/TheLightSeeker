// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return ASC.Get();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int32 ACharacterBase::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
	{
		return FMath::Floor(AttributeSet->GetLevel());
	}

	return 1;
}

float ACharacterBase::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

void ACharacterBase::OnRep_PlayerState()
{
	ALightSeekerPlayerState* PS = GetPlayerState<ALightSeekerPlayerState>();

	if (PS)
	{
		InitializeStartingValues(PS);
		//BindASCInput();
		InitializeAttributes();
	}

}

void ACharacterBase::InitializeStartingValues(ALightSeekerPlayerState* PS)
{
	ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSet = PS->GetAttributeSet();

	//AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	InitializeAttributes();


	AttributeSet->SetHealth(GetMaxHealth());
	AttributeSet->SetLevel(GetCharacterLevel());

}

void ACharacterBase::AddCharacterAbilities()
{
}

void ACharacterBase::InitializeAttributes()
{
}

void ACharacterBase::AddStartupEffects()
{
}

//void ACharacterBase::BindASCInput()
//{
//	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
//	{
//		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("DemoAbilityID"), static_cast<int32>(KidKingAbilityID::Confirm), static_cast<int32>(KidKingAbilityID::Cancel)));
//	}
//
//	ASCInputBound = true;
//
//}
//
//void ACharacterBase::InitializeStartingValues(AKidKingPlayerState* PS)
//{
//	AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());
//	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
//
//	AttributeSetBase = PS->GetAttributeSetBase();
//
//	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
//
//	InitializeAttributes();
//
//
//	AttributeSetBase->SetHealth(GetMaxHealth());
//	AttributeSetBase->SetStamina(GetMaxStamina());
//
//}

