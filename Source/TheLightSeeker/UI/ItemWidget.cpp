// Copyright (c) 2023 Team Light Seekers All rights reserved.

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
