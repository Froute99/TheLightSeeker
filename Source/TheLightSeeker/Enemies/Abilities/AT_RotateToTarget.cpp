// Copyright (c) 2023 Team Light Seekers All rights reserved.


#include "Enemies/Abilities/AT_RotateToTarget.h"
#include "AbilitySystemComponent.h"
#include "CharacterBase.h"
UAT_RotateToTarget::UAT_RotateToTarget()
{
	bTickingTask = true;
}

void UAT_RotateToTarget::Activate()
{
	if (!Target.IsValid())
	{
		EndTask();
	}
}

void UAT_RotateToTarget::TickTask(float DeltaTime)
{
	if (!Target.IsValid())
	{
		EndTask();
	}

	//UE_LOG(LogTemp, Log, TEXT("TickTask TEST, %s"), *AActor::GetDebugName());
	AActor* Owner = AbilitySystemComponent->GetOwner();
	
	FVector ToTarget = Target.Get()->GetActorLocation() - Owner->GetActorLocation();
	ToTarget.Z = 0.0f;
	
	Owner->SetActorRotation(ToTarget.Rotation());
}
