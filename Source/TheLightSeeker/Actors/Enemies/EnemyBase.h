// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h" // include ENEMYLOG
#include "Actors/Characters/CharacterBase.h"
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

	UFUNCTION(BlueprintCallable)
	void OnDeath();

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class UCharacterAbilitySystemComponent* ASC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class UCharacterAttributeSet* AttributeSet;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCharacterAttributeSet* GetAttributeSet() const;

	void HandleDamage(float Damage, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags,
		class ACharacterBase* SourceCharacter, class AActor* SourceActor);
	void HandleHealthChanged(float Value, const FGameplayTagContainer& SourceTags);

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> EnemyAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	void SetHealth(float Value);
	void SetMaxHealth(float Value);

	UFUNCTION(BlueprintCallable, Category = "Enemy|Abilities")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Enemy|Abilities")
	float GetMaxHealth() const;

	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	bool IsAlive() const;

	FDelegateHandle HealthChangedDelegateHandle;
};
