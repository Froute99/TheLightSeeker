// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

#include "CharacterBase.h"
#include "EnemyBase.h"

void UCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UCharacterAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Level, OldLevel);
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_DefaultDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, BasicDamage, OldValue);
}

void UCharacterAttributeSet::OnRep_DamageRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DamageMultiplier, OldValue);
}

void UCharacterAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MovementSpeed, OldValue);
}

void UCharacterAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackSpeed, OldValue);
}

void UCharacterAttributeSet::OnRep_AdditionalProjectiles(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AdditionalProjectiles, OldValue);
}

void UCharacterAttributeSet::OnRep_CooldownReduceRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, CooldownReduceRate, OldValue);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DISABLE_REPLICATED_PROPERTY(UCharacterAttributeSet, BasicDamage);
	DISABLE_REPLICATED_PROPERTY(UCharacterAttributeSet, DamageMultiplier);
	// DISABLE_REPLICATED_PROPERTY(UCharacterAttributeSet, MovementSpeed);
	DISABLE_REPLICATED_PROPERTY(UCharacterAttributeSet, AttackSpeed);
	DISABLE_REPLICATED_PROPERTY(UCharacterAttributeSet, AdditionalProjectiles);
	DISABLE_REPLICATED_PROPERTY(UCharacterAttributeSet, CooldownReduceRate);
}
