// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API AEnemyController : public AAIController 
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBlackboardData* BBAsset;
};
