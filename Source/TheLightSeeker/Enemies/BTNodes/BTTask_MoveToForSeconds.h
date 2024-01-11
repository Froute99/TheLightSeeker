// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToForSeconds.generated.h"

/**
 * Move To task node.
 * Moves the AI pawn toward the specified Actor or Location blackboard entry using the navigation system for specific time.
 */
UCLASS(config=Game)
class THELIGHTSEEKER_API UBTTask_MoveToForSeconds : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToForSeconds();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	void OnTimerExpired(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY(EditAnywhere)
	float Timer;

};
