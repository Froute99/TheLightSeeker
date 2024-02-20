// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"

#include "GameplayEffectTypes.h"

#include "LightSeekerPlayerState.generated.h"

/**
 *
 */

DECLARE_DELEGATE(FDoneElementalEffect)

UCLASS()
class THELIGHTSEEKER_API ALightSeekerPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALightSeekerPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UCharacterAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool IsAlive() const;

	// UFUNCTION(BlueprintCallable, Category = "PlayerState|Attributes")
	// void ShowAbilityConfirmCancelText(bool ShowText);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class USkillTreeComponent* SkillTreeComponent;

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "PlayerState|Attributes")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "PlayerState|Attributes")
	int32 GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UCharacterAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	class UCharacterAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle LevelChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void LevelChanged(const FOnAttributeChangeData& Data);
	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(Client, Reliable)
	void RepHealthBar(float NewValue);

	// virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:
	// Elemental Enchant gameplay effect handle
	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle ElementalEffectHandle;

	UFUNCTION(BlueprintCallable)
	void ClearElementalEffect();

	FDoneElementalEffect ElementalEffectDone;
};
