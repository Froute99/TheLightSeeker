// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CharacterGameplayAbility.h"
#include "GA_CharacterDefaultAttack.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API UGA_CharacterDefaultAttack : public UCharacterGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_CharacterDefaultAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MontageAbility)
		TObjectPtr<UAnimMontage> MontageToPlay;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<AActor> ArrowClass;

};
