// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthBarWidget.generated.h"

/**
 *
 */

UCLASS()
class THELIGHTSEEKER_API UPlayerHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealth(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;
};
