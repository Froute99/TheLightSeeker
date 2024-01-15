// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

/**
 *   UI for elite enemy health which displayed top of the elite enemy
 */
UCLASS()
class THELIGHTSEEKER_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMaxHealth(float MaxHealthValue, bool SetHealthFull = true);

	void SetHealth(float CurrentHealthValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;
};
