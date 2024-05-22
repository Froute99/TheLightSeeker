// Copyright (c) 2023 Team Light Seekers All rights reserved.


#include "Characters/Tombstone.h"
#include "Components/CapsuleComponent.h"
#include "CharacterBase.h"
#include "Characters/LightSeekerPlayerState.h"
#include "Components/AudioComponent.h"

ATombstone::ATombstone()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShapeComponent"));
	CapsuleComponent->SetupAttachment(GetStaticMeshComponent());

	// From Item.cpp - only interact with player
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetStaticMeshComponent());
}

void ATombstone::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATombstone::OnBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ATombstone::OnEndOverlap);

	IsWaitingDestroy = false;
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
			Character->ToggleReviveStatus(this, true);
		}
	}
}

void ATombstone::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		Character->ToggleReviveStatus(nullptr, false);
	}
}

// Called by Client Character
void ATombstone::Server_ToggleVFX_Implementation(bool IsTriggered)
{
	Multicast_ToggleVFX(IsTriggered);
}

// Called by Server
void ATombstone::Multicast_ToggleVFX_Implementation(bool IsTriggered)
{
	if (!IsWaitingDestroy)
	{
		ToggleVFX(IsTriggered);
	}
}

void ATombstone::OnPlayerRevived_Implementation()
{
	IsWaitingDestroy = true;

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetStaticMeshComponent()->SetVisibility(false);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]() {
		if (this)
		{
			this->Destroy();
		}
	});
	FTimerHandle   TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.0f, false);

	AudioComponent->Play();
}
