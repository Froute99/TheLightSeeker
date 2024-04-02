// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API UAT_RotateToTarget : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAT_RotateToTarget();

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	TWeakObjectPtr<AActor> Target;
};
