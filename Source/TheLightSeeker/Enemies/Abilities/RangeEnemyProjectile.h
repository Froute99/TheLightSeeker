// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheLightSeeker.h"
#include "Projectiles/ProjectileBase.h"
#include "Abilities/GameplayAbility.h"
#include "RangeEnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API ARangeEnemyProjectile : public AProjectileBase
{
	GENERATED_BODY()

	ARangeEnemyProjectile();

protected:
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	//FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION()
	void OnBeginOverlap_EnemyAttack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
