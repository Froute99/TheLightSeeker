// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSeekerPlayerState.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"

#include "Actors/Characters/CharacterBase.h"



ALightSeekerPlayerState::ALightSeekerPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;


}

UAbilitySystemComponent* ALightSeekerPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSet* ALightSeekerPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool ALightSeekerPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ALightSeekerPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float ALightSeekerPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

int32 ALightSeekerPlayerState::GetCharacterLevel() const
{
	return FMath::Floor(AttributeSet->GetLevel());
}

void ALightSeekerPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ALightSeekerPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ALightSeekerPlayerState::MaxHealthChanged);
		LevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetLevelAttribute()).AddUObject(this, &ALightSeekerPlayerState::LevelChanged);
	}
}

void ALightSeekerPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("%s : Player Health Changed"), *FString(__FUNCTION__));
}

void ALightSeekerPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("%s : Player Max Health Changed"), *FString(__FUNCTION__));
}

void ALightSeekerPlayerState::LevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : Player Level Changed!"), *FString(__FUNCTION__));
}

