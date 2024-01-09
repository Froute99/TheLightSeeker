// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterGameplayAbility.h"
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
		virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<AActor> ArrowClass;

	void SpawnProjectile();
	
	UFUNCTION(BlueprintCallable)
		virtual void AdditionalSpawnEvent() {}


	class UAT_PlayMontageAndWaitForEvent* TaskHandle;
	UFUNCTION(BlueprintCallable)
		void PlayAnim();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
		FGameplayTag AnimDoneTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
		FGameplayTag AnimTriggerTag;

};
