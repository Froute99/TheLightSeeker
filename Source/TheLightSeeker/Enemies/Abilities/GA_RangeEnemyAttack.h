// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h"
#include "EnemyGameplayAbility.h"
#include "GA_RangeEnemyAttack.generated.h"

/**
 *  Basic Attack Ability of Range type enemy
 */

UCLASS()
class THELIGHTSEEKER_API UGA_RangeEnemyAttack : public UEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void SetPlayerReference(AActor* Player);

protected:
	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileActor;

	AActor* PlayerReference;
};