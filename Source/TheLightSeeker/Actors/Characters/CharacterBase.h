// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"

#include "CharacterBase.generated.h"

UCLASS()
class THELIGHTSEEKER_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "View")
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "View")
		class UCameraComponent* Camera;

/************************
 * Game Abilities System
 ************************/

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Abilities")
		TWeakObjectPtr<class UCharacterAbilitySystemComponent> ASC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Abilities")
		TWeakObjectPtr<class UCharacterAttributeSet> AttributeSet;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character|Abilities")
		TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character|Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;


	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		int32 GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetMaxHealth() const;


	virtual void OnRep_PlayerState() override;
	void InitializeStartingValues(class ALightSeekerPlayerState* PS);


	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
