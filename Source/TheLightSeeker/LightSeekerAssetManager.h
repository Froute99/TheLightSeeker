// Copyright (c) 2023 Team Light Seekers All rights reserved.

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
