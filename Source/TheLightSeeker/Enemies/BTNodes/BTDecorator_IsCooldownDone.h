// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsCooldownDone.generated.h"

/**
 *	Node to check if cooldown is done and can activate ability.
 */
UCLASS()
class THELIGHTSEEKER_API UBTDecorator_IsCooldownDone : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsCooldownDone(const FObjectInitializer& ObjectInitializer);
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	float Cooldown;

	UPROPERTY(EditAnywhere)
	float RandomOffset;

public:
	float PreviousActivationTime;
	float TimeOffset;
};
