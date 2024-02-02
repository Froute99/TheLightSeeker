
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHPBarWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHPBarWidget::SetMaxHealth(float MaxHealthValue, bool SetHealthFull)
{
	MaxHealth = MaxHealthValue;
	if(SetHealthFull)
	{
		UE_LOG(LogTemp, Log, TEXT("Set max health: %f"), MaxHealth);
		SetHealth(MaxHealth);
	}
}

void UEnemyHPBarWidget::SetHealth(float CurrentHealthValue)
{
	if (HealthBar)
	{
		UE_LOG(LogTemp, Log, TEXT("Set Heatlh percent: %f"), CurrentHealthValue / MaxHealth);
		HealthBar->SetPercent(CurrentHealthValue / MaxHealth);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("shiba"));
	}
}