// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCharacterAttributeSet* GetAttributeSet() const override;

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

	UPROPERTY()
	class UCharacterAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	class UCharacterAttributeSet* AttributeSet;


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
