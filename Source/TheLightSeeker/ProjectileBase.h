// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectileBase.generated.h"

UCLASS()
class THELIGHTSEEKER_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
		class UProjectileMovementComponent* ProjectileMovementComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
		class UStaticMeshComponent* Mesh;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Effect")
		FGameplayEffectSpecHandle DamageEffectSpecHandle;

};
