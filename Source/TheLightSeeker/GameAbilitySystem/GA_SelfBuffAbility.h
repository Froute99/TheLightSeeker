// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/CharacterGameplayAbility.h"
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
