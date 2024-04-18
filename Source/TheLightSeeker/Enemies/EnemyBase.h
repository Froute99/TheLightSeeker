// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "TheLightSeeker.h" // include ENEMYLOG
#include "Characters/CharacterBase.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EnemyBase.generated.h"

/**
 *   A Base class for all kind of enemies
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyOnDiedDelegateHandle);

UCLASS()
class THELIGHTSEEKER_API AEnemyBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();

	class UBehaviorTree* GetBTAsset() const;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	float GetAttackRange() const;

	UFUNCTION()
	TWeakObjectPtr<USkeletalMeshComponent> GetWeaponMesh() const;

	UFUNCTION(BlueprintCallable)
	float GetScale() const;

	virtual void OnActivate();
	virtual void OnDeactivate();

	/***********************************************
	 * Death & Item Drop
	 ***********************************************/
	UFUNCTION(BlueprintCallable)
	void OnDied();
	FEnemyOnDiedDelegateHandle OnDiedDelegateHandle;

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintCallable)
	void FinishDying();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	FGameplayTag DeadTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TSubclassOf<class AItem> Item;

	/***********************************************
	 * Enemy Targeting
	 ***********************************************/

	void   SetTargetPlayer(APawn* Target);
	APawn* GetTargetPlayer() const;

	FTimerHandle TargetResetTimerHandle;
	APawn*		 TargetPlayer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	TObjectPtr<UAnimMontage> WeaponDeathAnimMontage;

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayWeaponAnimMontage(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RemoveCollision();

	/***********************************************
	 * Game Abilities System
	 ***********************************************/

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class UCharacterAbilitySystemComponent* ASC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class UCharacterAttributeSet* AttributeSet;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Blueprintcallable)
	virtual UCharacterAttributeSet* GetAttributeSet() const;

	void HandleDamage(float Damage, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags,
		class ACharacterBase* SourceCharacter, class AActor* SourceActor);
	void HandleHealthChanged(float Value, const FGameplayTagContainer& SourceTags);

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);

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

	bool IsDying;
	bool IsAlive() const;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;

	void RemoveCharacterAbilities();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* HPBar;

	// sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|SFX")
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|SFX")
	class USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|SFX")
	class USoundBase* HitSound;
};
