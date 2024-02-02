// Fill out your copyright notice in the Description page of Project Settings.

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
	void SpawnFloatingDamageTextWidget(UCharacterAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

};
