// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPBarWidgetComponent.h"
#include "Components/ProgressBar.h"

UHPBarWidgetComponent::UHPBarWidgetComponent()
{
	HealthBar = CreateDefaultSubobject<UProgressBar>(TEXT("HP Bar"));
}

void UHPBarWidgetComponent::SetMaxHealth(float MaxHealthValue, bool SetHealthFull)
{
	MaxHealth = MaxHealthValue;
	if (SetHealthFull)
	{
		SetHealth(MaxHealth);
	}
}

void UHPBarWidgetComponent::SetHealth(float CurrentHealthValue)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealthValue / MaxHealth);
	}
}