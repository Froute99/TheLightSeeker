// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/DamageEffectExecutionCalculation.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "AbilitySystemComponent.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefaultDamage);


	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, DefaultDamage, Source, false);

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
}

void UDamageEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogTemp, Error, TEXT("ExecTest"));
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

	//float BaseDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefaultDamageDef, EvaluationParameters, BaseDamage);

	UE_LOG(LogTemp, Warning, TEXT("BaseDamage: %f"), BaseDamage);
	float DamageDone = BaseDamage;

	if (DamageDone < 0.0f)		DamageDone = 0.0f;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -DamageDone));
}
