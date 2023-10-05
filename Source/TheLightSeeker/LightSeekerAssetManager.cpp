// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSeekerAssetManager.h"
#include "AbilitySystemGlobals.h"

void ULightSeekerAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
