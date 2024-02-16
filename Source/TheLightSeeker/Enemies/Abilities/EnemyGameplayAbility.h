// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h"
#include "Abilities/GameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API UEnemyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEnemyGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* AttackMontageForWeapon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	FDelegate_SetAbilityEnd SetAbilityDoneDelegateHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FScalableFloat CooldownDuration;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FGameplayTagContainer CooldownTags;
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

public:
	UFUNCTION()
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	virtual const FGameplayTagContainer* GetCooldownTags() const;
	virtual void						 ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

	UPROPERTY(EditDefaultsOnly)
	bool IsRequireTargetReference;

	// Set Target reference for ability, called by Behavior Tree
	void SetTargetReference(TWeakObjectPtr<AActor> Target);

protected:
	TWeakObjectPtr<AActor> AbilityTarget;
};
