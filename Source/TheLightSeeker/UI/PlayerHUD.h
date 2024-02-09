// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnFloatingDamageTextWidget(float Dmg);

	UFUNCTION(BlueprintCallable)
	bool BindDelegate();
	//void SpawnFloatingDamageTextWidget(UCharacterAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
