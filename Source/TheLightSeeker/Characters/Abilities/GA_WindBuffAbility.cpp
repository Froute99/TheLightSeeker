// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Characters/Abilities/GA_WindBuffAbility.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.h"

void UGA_WindBuffAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(ActorInfo->AbilitySystemComponent))
	{
		ASC->ApplyCooldownReduceForSkills(CooldownReduceRate);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not cast ASC to Character ASC"));
	}
}
