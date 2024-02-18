// Copyright (c) 2023 Team Light Seekers All rights reserved.

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	FString AbilityName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FScalableFloat CooldownDuration;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldowns")
	FGameplayTagContainer CooldownTags;
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	virtual const FGameplayTagContainer* GetCooldownTags() const;
	virtual void						 ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
};
