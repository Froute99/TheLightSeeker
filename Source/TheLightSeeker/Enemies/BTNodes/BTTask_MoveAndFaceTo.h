// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "TheLightSeeker.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveAndFaceTo.generated.h"

/**
 *	BTTask Node that moves to target for certain range & face angle
 */
UCLASS()
class THELIGHTSEEKER_API UBTTask_MoveAndFaceTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_MoveAndFaceTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void				TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	bool IsTaskDone(UBehaviorTreeComponent& OwnerComp) const;
	
	/* Actual Acceptable Radius - from MoveTo one will be "fake" value */
	UPROPERTY(EditAnywhere, Category = MoveAndFaceTo)
	float AcceptableRadiusForNode;

	UPROPERTY(EditAnywhere, Category = MoveAndFaceTo)
	float AcceptableFaceAngleDiff;
};
