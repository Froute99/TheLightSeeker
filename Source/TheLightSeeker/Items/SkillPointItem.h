// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "SkillPointItem.generated.h"

/**
 *
 */
UCLASS()
class THELIGHTSEEKER_API ASkillPointItem : public AItem
{
	GENERATED_BODY()

public:
	virtual void OnPickup(class ACharacterBase* Player) override;
};
