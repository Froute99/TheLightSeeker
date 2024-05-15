// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Tombstone.generated.h"

/**
 *    An Actor class for Player tombstone, which supports player revive functionality
 */
UCLASS()
class THELIGHTSEEKER_API ATombstone : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ATombstone();

	virtual void BeginPlay() override;

	void SetOwnerPlayer(TWeakObjectPtr<class ALightSeekerPlayerState> PS);
	TWeakObjectPtr<class ALightSeekerPlayerState> GetOwnerPlayer() const;


	// display UI for revive


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Collision

protected:
	TWeakObjectPtr<class ALightSeekerPlayerState> Owner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComponent;

};
