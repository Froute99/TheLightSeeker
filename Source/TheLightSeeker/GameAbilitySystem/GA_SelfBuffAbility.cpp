// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_SelfBuffAbility.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"

void UGA_SelfBuffAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Activated buff ability"));
	if (DamageGameplayEffect)
	{
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}
}