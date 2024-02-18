// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Abilities/EnemyGameplayAbility.h"
#include "GA_EliteEnemyRage.generated.h"

/**
 *   Ability of Elite enemy that grants buff for nearby enemies
 */
UCLASS()
class THELIGHTSEEKER_API UGA_EliteEnemyRage : public UEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

public:
	UPROPERTY(EditDefaultsOnly)
	float EffectRange;
};
