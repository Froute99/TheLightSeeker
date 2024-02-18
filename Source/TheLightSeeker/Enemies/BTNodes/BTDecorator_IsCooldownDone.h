// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "TheLightSeeker.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsCooldownDone.generated.h"

/**
 *	Node to check if cooldown is done and can activate ability AND STOP CURRENT BEHAVIOR
 */
UCLASS()
class THELIGHTSEEKER_API UBTDecorator_IsCooldownDone : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsCooldownDone(const FObjectInitializer& ObjectInitializer);
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	FGameplayTag CooldownTag;
};
