// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDefaultDamageEffect.h"
#include "CharacterAttributeSet.h"
#include "GameplayTagsManager.h"

UCharacterDefaultDamageEffect::UCharacterDefaultDamageEffect()
{
	FGameplayModifierInfo DamageInfo;

	DamageInfo.Attribute = UCharacterAttributeSet::GetBasicDamageAttribute();
	DamageInfo.ModifierOp = EGameplayModOp::Override;
	DamageInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(30.0f);
	Modifiers.Add(DamageInfo);

	FGameplayEffectCue DamageCue;
	FGameplayTagContainer DamageTagContainer;
	TArray<FString> Tags;
	Tags.Add("Gameplaycue.Attack");
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, DamageTagContainer);
	DamageCue.GameplayCueTags = DamageTagContainer;
	GameplayCues.Add(DamageCue);

}
