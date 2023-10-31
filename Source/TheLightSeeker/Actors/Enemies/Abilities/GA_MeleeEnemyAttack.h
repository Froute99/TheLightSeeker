// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h"
#include "Abilities/GameplayAbility.h"
#include "GA_MeleeEnemyAttack.generated.h"

/**
 *	Basic Attack Ability of Melee type enemy
 */

DECLARE_MULTICAST_DELEGATE(FDelegate_SetAbilityEnd);

UCLASS()
class THELIGHTSEEKER_API UGA_MeleeEnemyAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_MeleeEnemyAttack();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	FDelegate_SetAbilityEnd SetAbilityDoneDelegate;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
