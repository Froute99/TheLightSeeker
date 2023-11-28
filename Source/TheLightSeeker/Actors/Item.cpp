// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	if (BoxComponent)
	{
		SetRootComponent(BoxComponent);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);

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
	PS->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ItemAbility, 1, -1, this));
}

void AItem::Movement(float DeltaTime)
{
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() != ROLE_Authority) return;

	ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Cast<ACharacterBase>(OtherActor)->GetPlayerState());
	if (PS && ItemAbility)
	{
		OnPickup(PS);
	}
}

