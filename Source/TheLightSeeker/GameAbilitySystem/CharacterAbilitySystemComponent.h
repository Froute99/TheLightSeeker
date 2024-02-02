// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedDamageDelegate, float, DamageReceived);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceivedDamageDelegate,
//	UCharacterAbilitySystemComponent*, SourceASC, float, DamageReceived);

UCLASS()
class THELIGHTSEEKER_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectApplied = false;

	UPROPERTY(BlueprintAssignable)
	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(float DamageReceived);

	void LocalInputConfirm() override;
	void LocalInputCancel() override;
};
