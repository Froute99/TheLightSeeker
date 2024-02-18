// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "LightSeekerAssetManager.h"
#include "AbilitySystemGlobals.h"

void ULightSeekerAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
