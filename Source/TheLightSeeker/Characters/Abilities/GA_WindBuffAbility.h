// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/GA_SelfBuffAbility.h"
#include "GA_WindBuffAbility.generated.h"

/**
 *  Ability class for wind buff item
 */
UCLASS()
class THELIGHTSEEKER_API UGA_WindBuffAbility : public UGA_SelfBuffAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CooldownReduceRate;
};
