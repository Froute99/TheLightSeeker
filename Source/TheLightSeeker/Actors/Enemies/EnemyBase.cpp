// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyBase.h"
//#include "BehaviorTree/BehaviorTree.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* AEnemyBase::GetBTAsset() const
{
	return BTAsset;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
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

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSet* AEnemyBase::GetAttributeSet() const
{
	return AttributeSet;
}

float AEnemyBase::GetHealth() const
{
	return AttributeSet->GetMaxHealth();
}

void AEnemyBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent == nullptr || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartupAbility : EnemyAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, -1, this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AEnemyBase::InitializeAttributes()
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// effect context, handle
	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0.0f, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
	
	// set WalkSpeed based on attribute
	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();
}	
	
void AEnemyBase::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority || AbilitySystemComponent == nullptr || AbilitySystemComponent->StartupEffectApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0.0f, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->StartupEffectApplied = true;
}
