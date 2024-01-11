// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ItemWidget.h"
#include "Components/Image.h"

void UItemWidget::SetIcon(UTexture2D* Texture)
{
	if (Texture)
	{
		Icon->SetBrushFromTexture(Texture);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UI Could not find texture"));
	}
}

void UItemWidget::ClearIcon()
{
	Icon->SetBrushFromTexture(nullptr);
}
