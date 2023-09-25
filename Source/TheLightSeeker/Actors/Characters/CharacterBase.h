// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class THELIGHTSEEKER_API ACharacterBase : public ACharacter
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightSeeker|Abilities")
		TWeakObjectPtr<class UCharacterAbilitySystemComponent> ASC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightSeeker|Abilities")
		TWeakObjectPtr<class UCharacterAttributeSet> AttributeSet;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LightSeeker|Abilities")
		TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LightSeeker|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LightSeeker|Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;


	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetStamina() const;
	UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
		float GetMaxStamina() const;

	virtual void OnRep_PlayerState() override;
	void InitializeStartingValues(class APlayerState* PS);


	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
