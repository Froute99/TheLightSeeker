// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Bomb.generated.h"

UCLASS()
class THELIGHTSEEKER_API ABomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnSet();
	void Explode();

	bool IsSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bomb")
	float Radius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bomb")
	float Timer;

	FGameplayEffectSpecHandle BombGameplayEffectSpecHandle;
};
