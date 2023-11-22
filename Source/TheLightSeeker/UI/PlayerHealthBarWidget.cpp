// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHealthBarWidget::SetHealth(float Value)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Value / MaxHealth);
	}
}
