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

	virtual void BeginPlay() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWidgetComponent* HPBar;
};
