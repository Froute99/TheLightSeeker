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
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	FDelegate_SetAbilityEnd SetAbilityDoneDelegateHandle;


public:
	UFUNCTION()
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
