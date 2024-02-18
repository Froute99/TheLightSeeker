// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Abilities/CharacterGameplayAbility.h"
#include "GA_SelfBuffAbility.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API UGA_SelfBuffAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
