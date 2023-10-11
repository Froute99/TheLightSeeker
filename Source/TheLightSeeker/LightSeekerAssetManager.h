// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LightSeekerAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API ULightSeekerAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;

};