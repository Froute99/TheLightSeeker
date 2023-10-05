// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyBase.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (ASC.IsValid())
	{
		ASC->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		// health change handle (for future interface)
		// 
		// tag change callbacks
	}
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	if (ASC.IsValid())
	{
		return ASC.Get();
	}
	return nullptr;
}

float AEnemyBase::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

void AEnemyBase::AddCharacterAbilities()
{

}

void AEnemyBase::InitializeAttributes()
{
	if (!ASC.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// effect context, handle
}

void AEnemyBase::AddStartupEffects()
{
	ASC->StartupEffectApplied = false;
}
