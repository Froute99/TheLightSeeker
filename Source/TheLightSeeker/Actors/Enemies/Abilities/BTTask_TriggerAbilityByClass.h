// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TriggerAbilityByClass.generated.h"

/**
 *	Behavior Tree Task node to trigger pre-registered ability class of actor.
 */


UCLASS()
class THELIGHTSEEKER_API UBTTask_TriggerAbilityByClass : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TriggerAbilityByClass();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	UFUNCTION()
	void SetTaskDone();
	
	UPROPERTY(EditAnywhere, Category = Node)
	TSubclassOf<class UGameplayAbility> AbilityToActivate;

	UGameplayAbility* RunningAbility;

	bool IsTaskDone;
};
