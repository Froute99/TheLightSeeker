// Fill out your copyright notice in the Description page of Project Settings.

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

	void SetPlayerReference(AActor* Player);

protected:
	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UPROPERTY(EditDefaultsOnly)
	float LeapDuration;

	AActor* PlayerReference;
	FVector TargetLocation;
};
