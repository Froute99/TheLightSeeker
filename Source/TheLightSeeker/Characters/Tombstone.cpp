// Copyright (c) 2023 Team Light Seekers All rights reserved.


#include "Characters/Tombstone.h"
#include "Components/CapsuleComponent.h"
#include "CharacterBase.h"
#include "Characters/LightSeekerPlayerState.h"

ATombstone::ATombstone()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShapeComponent"));
	CapsuleComponent->SetupAttachment(GetStaticMeshComponent());

	// From Item.cpp - only interact with player
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void ATombstone::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATombstone::OnBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ATombstone::OnEndOverlap);
}

void ATombstone::SetOwnerPlayer(TWeakObjectPtr<class ALightSeekerPlayerState> PS)
{
	Owner = PS;
}

TWeakObjectPtr<class ALightSeekerPlayerState> ATombstone::GetOwnerPlayer() const
{
	return Owner;
}

void ATombstone::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		if (!Character->IsDead)
		{
			Character->ToggleReviveStatus(true);
		}
	}
}

void ATombstone::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		Character->ToggleReviveStatus(false);
	}
}
