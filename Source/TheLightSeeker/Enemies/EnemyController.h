// Copyright (c) 2023 Team Light Seekers All rights reserved.

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
	AEnemyController(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBlackboardData* BBAsset;

	UFUNCTION(BlueprintCallable)
	bool BindDelegate();

	UFUNCTION(BlueprintImplementableEvent)
	void FloatingDamage(class UCharacterAbilitySystemComponent* SourceASC,
		class UCharacterAbilitySystemComponent* TargetASC, float Value);
};
