// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "GameAbilitySystem/DamageEffectExecutionCalculation.h"

#include "CharacterAbilitySystemComponent.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "AbilitySystemComponent.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BasicDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMultiplier);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, BasicDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, DamageMultiplier, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

UDamageEffectExecutionCalculation::UDamageEffectExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().BasicDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageMultiplierDef);
}

void UDamageEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor*					 TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor*					 SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec&	 Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayTagContainer Container;
	Spec.GetAllAssetTags(Container);

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BasicDamageDef, EvaluationParameters, BaseDamage);

	float DamageDone = BaseDamage;

	float DamageRate = 0.f;
	if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageMultiplierDef, EvaluationParameters, DamageRate))
	{
		if (DamageRate <= 0.f)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Damage Percentage was 0 or below"), *FString(__FUNCTION__));
		}
		else
		{
			DamageDone *= DamageRate;
		}
	}

	if (DamageDone < 0.0f)
		DamageDone = 0.0f;

	double Randomizer = FMath::FRandRange(0.8, 1.2);
	DamageDone *= Randomizer;

	DamageDone = FMath::RoundToFloat(DamageDone);

	// Broadcast damage given
	Cast<UCharacterAbilitySystemComponent>(TargetASC)->ReceivedDamage(Cast<UCharacterAbilitySystemComponent>(SourceASC),
		Cast<UCharacterAbilitySystemComponent>(TargetASC), DamageDone);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -DamageDone));
}
