// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCharacterGameplayAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
		float Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
		float Damage;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;



	//UFUNCTION()
	//	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	//UFUNCTION()
	//	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);


};
