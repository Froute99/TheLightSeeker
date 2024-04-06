// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageReceivedSignature,
	UCharacterAbilitySystemComponent*, SourceASC,
	UCharacterAbilitySystemComponent*, TargetASC,
	float, DamageReceived);

UCLASS()
class THELIGHTSEEKER_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectApplied = false;

	UPROPERTY(BlueprintAssignable)
	FOnDamageReceivedSignature OnReceivedDamage;

	virtual void ReceivedDamage(UCharacterAbilitySystemComponent* SourceASC,
		UCharacterAbilitySystemComponent* TargetASC, float DamageAmount);

	void LocalInputConfirm() override;
	void LocalInputCancel() override;

	void ApplyCooldownReduceForSkills(float ReduceRate);
};
