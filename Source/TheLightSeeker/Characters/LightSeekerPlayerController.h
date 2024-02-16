// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LightSeekerPlayerController.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API ALightSeekerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void BeginPlay();

	UFUNCTION(BlueprintCallable)
	bool BindDelegate();

	UFUNCTION(BlueprintImplementableEvent)
	void FloatingDamage(class UCharacterAbilitySystemComponent* SourceASC,
		class UCharacterAbilitySystemComponent* TargetASC, float Value);
};
