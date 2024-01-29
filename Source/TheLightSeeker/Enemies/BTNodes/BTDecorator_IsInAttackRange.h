// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTDecorator_IsInAttackRange.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API UBTDecorator_IsInAttackRange : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInAttackRange(const FObjectInitializer& ObjectInitializer);
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	/*
	 *   Angle to check against face vector in Degree.
	 */
	UPROPERTY(EditAnywhere, Category = Node)
	float AllowableFaceAngleDiff;

	UPROPERTY(EditAnywhere, Category = Node)
	bool UseCustomRange;

	UPROPERTY(EditAnywhere, Category = Node)
	float CustomRange;
};
