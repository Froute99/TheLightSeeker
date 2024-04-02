// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "LightningTransferProjectile.generated.h"

UCLASS()
class THELIGHTSEEKER_API ALightningTransferProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightningTransferProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap_Override(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	TArray<class AEnemyBase*> TargetEnemies;

	FGameplayEffectSpecHandle DamageEffectSpecHandle;

private:
	int NextTargetIndex;
};
