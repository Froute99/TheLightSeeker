// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TheLightSeeker.h"
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

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;



	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		ELSAbilityInputID AbilityInputID = ELSAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		ELSAbilityInputID AbilityID = ELSAbilityInputID::None;

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		bool ActivateAbilityOnGranted = false;



};
