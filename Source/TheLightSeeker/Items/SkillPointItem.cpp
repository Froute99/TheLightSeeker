// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Items/SkillPointItem.h"
#include "Characters/CharacterBase.h"
#include "Characters/SkillTreeComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/LightSeekerPlayerState.h"

void ASkillPointItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.Clear(); // clear original item overlap function
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASkillPointItem::OnBeginOverlap_Skillpoint);
}

void ASkillPointItem::OnPickup(ACharacterBase* Player)
{
	if (ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Player->GetPlayerState()))
	{
		PS->SkillTreeComponent->IncreaseSkillPoint();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Skillpoint item : PS not available"));
	}
}

void ASkillPointItem::OnBeginOverlap_Skillpoint(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (ACharacterBase* Player = Cast<ACharacterBase>(OtherActor))
	{
		OnPickup(Player);
	}
	Destroy();
}
