// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/CharacterGameplayAbility.h"
#include "CharacterAnimAbility.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API UCharacterAnimAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

public:
	UCharacterAnimAbility();

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

	UFUNCTION(BlueprintCallable)
		void PlayAnim();
	class UAT_PlayMontageAndWaitForEvent* TaskHandle;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EventTags")
		FGameplayTag AnimDoneTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EventTags")
		FGameplayTag AnimTriggerTag;

};
