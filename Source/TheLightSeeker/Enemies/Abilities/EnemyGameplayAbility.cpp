// Copyright (c) 2023 Team Light Seekers All rights reserved.

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

const FGameplayTagContainer* UEnemyGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UEnemyGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	int32			 Level = GetAbilityLevel();
	float			 Cooldown = CooldownDuration.GetValueAtLevel(Level);

	if (CooldownGE && (Cooldown > 0.0f))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(CooldownTags.GetByIndex(0), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

void UEnemyGameplayAbility::SetTargetReference(TWeakObjectPtr<AActor> Target)
{
	AbilityTarget = Target;
}
