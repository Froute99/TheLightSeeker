// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "SkillPointItem.generated.h"

/**
 *	A class for skill point item
 */

UCLASS()
class THELIGHTSEEKER_API ASkillPointItem : public AItem
{
	GENERATED_BODY()

public:
	virtual void		 BeginPlay() override;
	virtual void OnPickup(class ACharacterBase* Player) override;

	UFUNCTION()
	void OnBeginOverlap_Skillpoint(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
