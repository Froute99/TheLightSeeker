// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyBase.h"
//#include "BehaviorTree/BehaviorTree.h"
#include "Actors/Characters/CharacterBase.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"

#include "Actors/Characters/LightSeekerPlayerState.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

UBehaviorTree* AEnemyBase::GetBTAsset() const
{
	return BTAsset;
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

float AEnemyBase::GetAttackRange() const
{
	return AttackRange;
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Log, TEXT("Enemy PoessedBy"));

	//ALightSeekerPlayerState* PS = NewController->GetPlayerState<ALightSeekerPlayerState>();
	ALightSeekerPlayerState* PS = GetPlayerState<ALightSeekerPlayerState>();
	if (PS)	
	{
		UE_LOG(LogTemp, Log, TEXT("PS is valid"));

		//InitializeStartingValues(PS);
		ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		AttributeSet = PS->GetAttributeSet();

		//InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();
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

void AEnemyBase::HandleDamage(float Damage, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, ACharacterBase* SourceCharacter, AActor* SourceActor)
{
	FString Name = SourceCharacter->GetName();
	UE_LOG(LogTemp, Log, TEXT("Damaged from: %s"), *Name);
}

void AEnemyBase::HandleHealthChanged(float Value, const FGameplayTagContainer& SourceTags)
{
	UE_LOG(LogTemp, Log, TEXT("Health Changed"));
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
