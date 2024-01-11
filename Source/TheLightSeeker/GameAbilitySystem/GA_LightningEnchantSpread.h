// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_LightningEnchantSpread.generated.h"

/**
 *	Ability Activated on enemy when getting damage from lightning enchant, for effect spreading
 */
UCLASS()
class THELIGHTSEEKER_API UGA_LightningEnchantSpread : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	int32 MaxTargetNum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	float SpreadRange;
};
