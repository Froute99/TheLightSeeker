// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HPBarWidgetComponent.generated.h"

/**
 *  Component for HP Bar displayed top of the enemy
 */
UCLASS()
class THELIGHTSEEKER_API UHPBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHPBarWidgetComponent();

	void SetMaxHealth(float MaxHealthValue, bool SetHealthFull = true);

	void SetHealth(float CurrentHealthValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;
};
