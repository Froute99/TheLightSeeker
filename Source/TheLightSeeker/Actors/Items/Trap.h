// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Trap.generated.h"

UCLASS()
class THELIGHTSEEKER_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroySelf();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	FGameplayEffectSpecHandle TrapGameplayEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trap")
	float LifeTime;

	TSet<TPair<class AEnemyBase*, FActiveGameplayEffectHandle>> EnemiesUnderEffect;
};
