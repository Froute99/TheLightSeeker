// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

/**
 * Character Base Attribute Set
 */


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class THELIGHTSEEKER_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;



	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_Level)
		FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Level);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);


	UPROPERTY(BlueprintReadOnly, Category = "DamageRate", ReplicatedUsing = OnRep_DamageRate)
		FGameplayAttributeData DamageRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DamageRate);

	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MovementSpeed)
		FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "AttackSpeed", ReplicatedUsing = OnRep_AttackSpeed)
		FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "AdditionalProjectiles", ReplicatedUsing = OnRep_AdditionalProjectiles)
		FGameplayAttributeData AdditionalProjectiles;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AdditionalProjectiles);

	// Need Enum
	//UPROPERTY(BlueprintReadOnly, Category = "ElementalEnforced", ReplicatedUsing = OnRep_ElementalEnforced)
	//	FGameplayAttributeData ElementalEnforced;
	//ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ElementalEnforced);



	UFUNCTION()
		virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);
	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
		virtual void OnRep_DamageRate(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void OnRep_AdditionalProjectiles(const FGameplayAttributeData& OldValue);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
