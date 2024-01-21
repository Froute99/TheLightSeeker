// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "BossEnemy.generated.h"

class UEnemyHPBarWidget;

/**
 * Base Class for Boss Enemy
 */
UCLASS()
class THELIGHTSEEKER_API ABossEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;

	UPROPERTY(EditAnywhere)
	UEnemyHPBarWidget* HPBar;
};
