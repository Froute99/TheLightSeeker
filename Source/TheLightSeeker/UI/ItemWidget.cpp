// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"
#include "Components/Image.h"

void UItemWidget::SetIcon(UTexture2D* Texture)
{
	Icon->SetBrushFromTexture(Texture);
}

void UItemWidget::ClearIcon()
{
	Icon->SetBrushFromTexture(ClearTexture);
}
