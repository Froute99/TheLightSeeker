// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "UI/EnemyHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Net/UnrealNetwork.h"

void UEnemyHPBarWidget::SetMaxHealth(float MaxHealthValue, bool SetHealthFull)
{
	MaxHealth = MaxHealthValue;
	SetVisibility(ESlateVisibility::Visible);
	if (SetHealthFull)
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
}