// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "InputAction.h"
#include "GameplayAbilitySpec.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReviveTriggerDelegate, bool, IsTriggered);

UCLASS()
class THELIGHTSEEKER_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	virtual void PossessedBy(AController* NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void		 EnhancedMove(const FInputActionValue& Value);
	void		 EnhancedLook(const FInputActionValue& Value);
	void		 CameraZoom(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View")
	float MinZoomLength = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View")
	float MaxZoomLength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View")
	float DefaultArmLength = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View")
	float ZoomStep = 40.f;

	/************************
	 * Game Abilities System
	 ************************/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> ASC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TWeakObjectPtr<class UCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	TWeakObjectPtr<class USkillTreeComponent> SkillTreeComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	void SetCharacterLevel(float Value);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetHealth(float Value);
	void SetMaxHealth(float Value);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int32 GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetMaxHealth() const;

	virtual void OnRep_PlayerState() override;
	void		 InitializeStartingValues(class ALightSeekerPlayerState* PS);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	void Attack();

	void UseAbility(int Index);
	void Ability1();
	void Ability2();
	void Ability3();
	void Ability4();
	void Dodge();

	bool IsDoingTargeting = false;

	/************************
	 * Input
	 ************************/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ZoomAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* DodgeAction;

	// Test
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* SkillAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* Skill1Action;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* Skill2Action;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* Skill3Action;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* Skill4Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ConfirmAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ReviveAction;

	/************************
	 * Item
	 ************************/
public:
	void OnPickupItem(TSubclassOf<class UCharacterGameplayAbility> ItemAbility, UTexture2D* Icon);

	void UseItem();

	UFUNCTION(Server, Reliable)
	void Server_UseItem();

	UFUNCTION(Client, Reliable)
	void Client_UpdateItemUI(UTexture2D* Texture);

	UFUNCTION(Client, Reliable)
	void Client_OnUseItemFailed();

	FGameplayAbilitySpecHandle ItemAbilityHandle;

	USoundBase* ItemPickupSound;
	USoundBase* ItemUseFailureSound;

protected:
	bool HasItem;

	/************************
	 * UI
	 ************************/
public:
	// This member is temporary here. Should moved to HUD class or something.
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, ReplicatedUsing = OnRep_HealthBar)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPlayerHealthBarWidget* HealthBar;

	UFUNCTION()
	void UpdateHealthBar();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UItemWidget* ItemWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UEnemyHPBarWidget* BossHPWidget;

	UFUNCTION()
	void InitializeBossHealthBar(float MaxHealth);

	UFUNCTION()
	void UpdateBossHealthBar(float CurrentHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UUserWidget* ReviveInstructionWidget;

	/************************
	 * Dead & Revive
	 ************************/

	UFUNCTION(NetMulticast, Reliable)
	void Die();

	void Revive(bool IsTriggered);

	UFUNCTION(Server, Reliable)
	void Server_Revive();

	UFUNCTION(Server, Reliable)
	void Server_TriggerReviveVFX(bool IsTriggered);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRevived();

	FTimerHandle ReviveCallTimerHandle;
	
	UPROPERTY(BlueprintAssignable)
	FReviveTriggerDelegate ReviveTriggeredDelegateHandle;

	void ToggleReviveStatus(TWeakObjectPtr<class ATombstone> TombstoneActor, bool CanRevive);

	bool CanRevivePlayer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TimerForRevive;

	TWeakObjectPtr<class ATombstone> Tombstone;

	// info for rollback status
	ECollisionEnabled::Type CollisionEnabled;
	float					GravityScale;

	UPROPERTY(BlueprintReadWrite)
	bool IsDead = false;

	int CurrentUsingAbilityIndex;
	int MaxAbilityNum = 3;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void IncreaseSkillPoint();
	UFUNCTION(BlueprintCallable)
	int GetSkillPointNum();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void GrantAbility(TSubclassOf<class UGameplayAbility> AbilityClass);

	UFUNCTION(Client, Reliable)
	void RepSkillPointSubtract(class UCharacterAnimAbility* Ability, int SkillPoint);
};
