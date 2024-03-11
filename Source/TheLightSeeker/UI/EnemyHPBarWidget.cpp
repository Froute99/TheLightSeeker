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
		SetHealth(MaxHealth);
	}
}

void UEnemyHPBarWidget::SetHealth(float CurrentHealthValue)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealthValue / MaxHealth);
	}
}