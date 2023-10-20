// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "MeleeAttackCollisionBox.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API UMeleeAttackCollisionBox : public UBoxComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetCollisionBox(bool IsSet);

	bool IsEnabled;
	float LifeTime;
};
