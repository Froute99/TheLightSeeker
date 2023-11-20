// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/CharacterGameplayAbility.h"

UCharacterGameplayAbility::UCharacterGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UCharacterGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActorInfo->OwnerActor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Owner Actor is valid: %s"), *ActorInfo->OwnerActor.Get()->GetName());
	}


}

void UCharacterGameplayAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

