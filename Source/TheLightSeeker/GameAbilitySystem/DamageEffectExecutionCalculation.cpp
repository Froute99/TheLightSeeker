// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/DamageEffectExecutionCalculation.h"

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

	if (DamageDone < 0.0f) DamageDone = 0.0f;

	double Randomizer = FMath::FRandRange(0.8, 1.2);
	DamageDone *= Randomizer;
	UE_LOG(LogTemp, Log, TEXT("Damage: %f"), DamageDone);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -DamageDone));

}
