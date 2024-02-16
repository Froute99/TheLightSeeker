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
