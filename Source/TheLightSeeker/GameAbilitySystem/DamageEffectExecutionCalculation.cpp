// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/DamageEffectExecutionCalculation.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "AbilitySystemComponent.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefaultDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageRate);


	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, DefaultDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, DamageRate, Source, false);

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
	RelevantAttributesToCapture.Add(DamageStatics().DefaultDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageRateDef);
}

void UDamageEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayTagContainer Container;
	Spec.GetAllAssetTags(Container);
	for (auto tag : Container)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), tag.GetTagName());
	}

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefaultDamageDef, EvaluationParameters, BaseDamage);

	UE_LOG(LogTemp, Log, TEXT("BaseDamage: %f"), BaseDamage);
	float DamageDone = BaseDamage;

	float DamagePercent = 0.f;
	if (ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageRateDef, EvaluationParameters, DamagePercent))
	{
		if (DamagePercent <= 0.f)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Damage Percentage was 0 or below"), *FString(__FUNCTION__));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Damage Percentage: %f"), DamagePercent);
			DamageDone *= (DamagePercent / 100.f);
		}
	}

	if (DamageDone < 0.0f) DamageDone = 0.0f;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -DamageDone));

}
