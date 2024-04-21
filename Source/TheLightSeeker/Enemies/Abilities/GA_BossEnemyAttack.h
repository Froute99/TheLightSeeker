// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Abilities/EnemyGameplayAbility.h"
#include "GA_BossEnemyAttack.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API UGA_BossEnemyAttack : public UEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "EnemyGameplayAbility")
	TSubclassOf<UGameplayEffect> AdditionalDamageGameplayEffect;
};
