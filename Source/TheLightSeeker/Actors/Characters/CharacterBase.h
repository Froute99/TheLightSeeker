// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "InputAction.h"

#include "CharacterBase.generated.h"

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
	void EnhancedMove(const FInputActionValue& Value);
	void EnhancedLook(const FInputActionValue& Value);
	void CameraZoom(const FInputActionValue& Value);


protected:
	// Called when the game starts or when spawned
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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
		TWeakObjectPtr<class UCharacterAbilitySystemComponent> ASC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
		TWeakObjectPtr<class UCharacterAttributeSet> AttributeSet;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;


	void SetCharacterLevel(float Value);
	void SetHealth(float Value);
	void SetMaxHealth(float Value);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		int32 GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		float GetMaxHealth() const;


	virtual void OnRep_PlayerState() override;
	void InitializeStartingValues(class ALightSeekerPlayerState* PS);


	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* Skill1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* Skill2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* Skill3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* Skill4Action;



};
