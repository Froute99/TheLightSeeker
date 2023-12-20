// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CharacterGameplayAbility.h"
#include "GA_SpawnBombAbility.generated.h"

/**
 * A GameAbility class to spawn a bomb item
 */
UCLASS()
class THELIGHTSEEKER_API UGA_SpawnBombAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MontageAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bomb")
	TSubclassOf<AActor> BombActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bomb")
	TSubclassOf<class UGameplayEffect> BombGameplayEffect;
};
