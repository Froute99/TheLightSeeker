// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGameplayAbility.h"

UEnemyGameplayAbility::UEnemyGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEnemyGameplayAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	SetAbilityDoneDelegateHandle.Broadcast();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UEnemyGameplayAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	SetAbilityDoneDelegateHandle.Broadcast();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyGameplayAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
