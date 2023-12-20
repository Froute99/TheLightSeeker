// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "CharacterBase.h"
#include "Abilities/CharacterGameplayAbility.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	if (BoxComponent)
	{
		SetRootComponent(BoxComponent);
		BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);


		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		Mesh->SetupAttachment(BoxComponent);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

void AItem::OnPickup(ACharacterBase* Player)
{
	Player->OnPickupItem(ItemAbility, Icon);
	//PS->GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ItemAbility, 1, -1, this));
}

void AItem::Movement(float DeltaTime)
{
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Item Picked 1"));
	if (GetLocalRole() != ROLE_Authority) return;

	ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
	if (Player && ItemAbility)
	{
		UE_LOG(LogTemp, Log, TEXT("Item Picked Up"));
		OnPickup(Player);
		Destroy();
	}
}

