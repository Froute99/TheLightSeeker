// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Abilities/EnemyGameplayAbility.h"
#include "GA_BossEnemyLeap.generated.h"

/**
 *   Leap Ability for Boss Enemy
 */
UCLASS()
class THELIGHTSEEKER_API UGA_BossEnemyLeap : public UEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UPROPERTY(EditDefaultsOnly)
	float LeapDuration;

	FVector TargetLocation;
};
