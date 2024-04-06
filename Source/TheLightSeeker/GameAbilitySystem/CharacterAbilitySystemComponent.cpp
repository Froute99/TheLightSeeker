// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "CharacterBase.h"
#include "PlayerHUD.h"

void UCharacterAbilitySystemComponent::ReceivedDamage(UCharacterAbilitySystemComponent* SourceASC,
	UCharacterAbilitySystemComponent* TargetASC, float DamageAmount)
{
	OnReceivedDamage.Broadcast(SourceASC, TargetASC, DamageAmount);
}

void UCharacterAbilitySystemComponent::LocalInputConfirm()
{
	Super::LocalInputConfirm();

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActor());
	Character->IsDoingTargeting = false;
}

void UCharacterAbilitySystemComponent::LocalInputCancel()
{
	Super::LocalInputCancel();

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActor());
	Character->IsDoingTargeting = false;
}

void UCharacterAbilitySystemComponent::ApplyCooldownReduceForSkills(float ReduceRate)
{	
	FGameplayTagContainer TagContainer;
	FGameplayTagContainer CooldownTagContainer = FGameplayTag::RequestGameplayTag(TEXT("Data.Cooldown")).GetSingleTagContainer();
	GetOwnedGameplayTags(TagContainer);

	for (int i = 0; i < TagContainer.Num(); ++i)
	{
		FGameplayTag Tag = TagContainer.GetByIndex(i);
		UE_LOG(LogTemp, Warning, TEXT("Tag found: %s"), *Tag.ToString());
	}

	for (const FActiveGameplayEffect& ActiveGE : &ActiveGameplayEffects)
	{
		if (ActiveGE.Spec.DynamicGrantedTags.HasAny(CooldownTagContainer))
		{
			ActiveGameplayEffects.ModifyActiveEffectStartTime(ActiveGE.Handle, -ActiveGE.GetDuration() * ReduceRate);
			UE_LOG(LogTemp, Warning, TEXT("ActiveGE Tag found: %s"), *ActiveGE.Spec.DynamicGrantedTags.ToString());
		}
	}
}
