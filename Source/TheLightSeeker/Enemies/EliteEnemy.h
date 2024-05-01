// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EliteEnemy.generated.h"

class UWidgetComponent;

/**
 *	Base Class for Elite Enemy
 */
UCLASS()
class THELIGHTSEEKER_API AEliteEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEliteEnemy();

	virtual void DropItem() override;
};
