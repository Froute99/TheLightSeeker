// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "EnemyBase.generated.h"

/**
 *   A Base class for all kind of enemies
 */

UCLASS()
class THELIGHTSEEKER_API AEnemyBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();

	class UBehaviorTree* GetBTAsset() const;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	float GetAttackRange() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BTAsset;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	float AttackRange;

/************************
 * Game Abilities System
 ************************/

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	void HandleDamage(float Damage, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags,
		class ACharacterBase* SourceCharacter, class AActor* SourceActor);

	void HandleHealthChanged(float Value, const FGameplayTagContainer& SourceTags);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|Abilities")
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|Abilities")
	TWeakObjectPtr<class UCharacterAttributeSet> AttributeSet;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> EnemyAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Abilities")
	float GetHealth() const;

	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();
};
