// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class THELIGHTSEEKER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

public:
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnPickup(class ACharacterBase* Player);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UTexture2D* Icon;

protected:
	void Movement(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UCharacterGameplayAbility> ItemAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//class UBoxComponent* BaseHeight;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//class USphereComponent* PickupCollision;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item|Movement")
	//float Height;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item|Movement")
	//float PickupRange;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item|Movement")
	//float FloatingRange;
	//
	///* Floating Rate per second */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item|Movement")
	//float FloatingSpeed;
	//
	///* Rotation Rate per second in degree */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item|Movement")
	//float RotationRate;
};
