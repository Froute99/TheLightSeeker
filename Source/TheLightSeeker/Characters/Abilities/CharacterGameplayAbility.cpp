// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "CharacterGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "CharacterBase.h"
#include "LightSeekerPlayerState.h"

UCharacterGameplayAbility::UCharacterGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCharacterGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

const FGameplayTagContainer* UCharacterGameplayAbility::GetCooldownTags() const
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

void UCharacterGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	int32			 Level = GetAbilityLevel();
	float			 Cooldown = CooldownDuration.GetValueAtLevel(Level);
	float			 CooldownReduceRate = 0.0f;

	if (ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(ActorInfo->OwnerActor)) // confirmed owner is playerstate
	{
		CooldownReduceRate = PS->GetAttributeSet()->GetCooldownReduceRate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UCharacterGameplayAbility::OwnerActor to ALightSeekerPlayerState cast failed"));
	}

	if (CooldownGE && (Cooldown > 0.0f))
	{
		float					  FinalCooldown = Cooldown * (1.0f - CooldownReduceRate);
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), Level);
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		// RequestDirectParent(): Change tag "Data.Cooldown.SkillName" to "Data.Cooldown",
		// so that the the can be matched with GE cooldown SetByCaller tag
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(CooldownTags.GetByIndex(0).RequestDirectParent(), FinalCooldown);
		auto EffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}
