// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Enemies/BTNodes/BTDecorator_IsCooldownDone.h"
#include "Math/UnrealMathUtility.h"

UBTDecorator_IsCooldownDone::UBTDecorator_IsCooldownDone(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is Cooldown for ability done and can activate it?";
	PreviousActivationTime = 0.0f;
}

bool UBTDecorator_IsCooldownDone::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	float Now = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());

	// can activate ability
	if ((Now - PreviousActivationTime) > (Cooldown + TimeOffset))
	{
		bResult = true;
		//TimeOffset = FMath::RandRange(-RandomOffset, RandomOffset);
		//PreviousActivationTime = Now;
	}
	else
	{
		bResult = false;
	}


	return bResult;
}
