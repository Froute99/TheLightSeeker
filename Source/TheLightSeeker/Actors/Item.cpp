// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(Mesh);

	//BaseHeight = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseHeight"));
	//BaseHeight->SetupAttachment(Mesh);
	//
	//PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
	//PickupCollision->SetupAttachment(Mesh);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	//BaseHeight->SetWorldScale3D({ 1.0f, 1.0f, Height });
	//PickupCollision->SetSphereRadius(PickupRange);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Movement(DeltaTime);
}

void AItem::OnPickup(ALightSeekerPlayerState* PS)
{
}

void AItem::Movement(float DeltaTime)
{
}

