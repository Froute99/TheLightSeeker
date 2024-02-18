// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetIcon(UTexture2D* Texture);

	UFUNCTION()
	void ClearIcon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// class UTexture2D* ClearTexture;
};
