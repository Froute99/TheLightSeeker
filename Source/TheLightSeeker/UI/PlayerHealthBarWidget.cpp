// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "UI/PlayerHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHealthBarWidget::SetHealth(float Value)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Value / MaxHealth);
	}
}
