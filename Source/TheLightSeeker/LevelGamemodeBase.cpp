// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "LevelGamemodeBase.h"
#include "CharacterBase.h"
#include "LightSeekerPlayerState.h"
#include "Tombstone.h"

ALevelGamemodeBase::ALevelGamemodeBase()
{
	HasGameStarted = false;
	CurrentPlayerCount = 0;
}

void ALevelGamemodeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// already rejected for some reason
	if (!ErrorMessage.IsEmpty())
	{
		return;
	}

	if (HasGameStarted)
	{
		ErrorMessage = FString("The Game has already started!");
		return;
	}

	if (CurrentPlayerCount == MaxPlayerCount)
	{
		ErrorMessage = FString("The Game is full!");
		return;
	}

	++CurrentPlayerCount;
}

void ALevelGamemodeBase::SetGameStart()
{
	HasGameStarted = true;
}

void ALevelGamemodeBase::OnPlayerDeath(TWeakObjectPtr<class ALightSeekerPlayerState> PS, FTransform Transform)
{
	UE_LOG(LogTemp, Verbose, TEXT("ALevelGamemodeBase::OnPlayerDeath called"));
	if (TombstoneBP)
	{
		SpawnedTombstone = GetWorld()->SpawnActor<ATombstone>(TombstoneBP, Transform);
		SpawnedTombstone->SetOwnerPlayer(PS);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState: Tombstone class not selected in BP"));
	}
}

void ALevelGamemodeBase::OnPlayerRevive()
{
	UE_LOG(LogTemp, Verbose, TEXT("ALevelGamemodeBase::OnPlayerRevive called"));
	if (SpawnedTombstone.IsValid())
	{
		TWeakObjectPtr<class ALightSeekerPlayerState> PlayerToRevive = SpawnedTombstone->GetOwnerPlayer();
		if (PlayerToRevive.IsValid())
		{
			PlayerToRevive->OnRevived();
			SpawnedTombstone.Get()->OnPlayerRevived();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ALevelGamemodeBase::OnPlayerRevive failed"));
		}
	}
}
