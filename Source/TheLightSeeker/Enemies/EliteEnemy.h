// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EliteEnemy.generated.h"

class UWidgetComponent;

/**
 *	Base Class for Elite Enemy
 */
UCLASS()
class THELIGHTSEEKER_API AEliteEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEliteEnemy();

	virtual void BeginPlay() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* HPBar;
};
