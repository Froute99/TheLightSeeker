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

void ALevelGamemodeBase::Logout(AController* Exiting)
{
	// Reduce the number of current player and adjust live player count if needed.
	--CurrentPlayerCount;
	LivePlayerCount = (LivePlayerCount < CurrentPlayerCount) ? LivePlayerCount : CurrentPlayerCount;
	
	if (HasGameStarted && CurrentPlayerCount == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("All player left the game. Reset the level."));
		ResetLevel();
		HasGameStarted = false;
	}
}

void ALevelGamemodeBase::SetGameStart()
{
	HasGameStarted = true;
	LivePlayerCount = CurrentPlayerCount;
}

void ALevelGamemodeBase::OnPlayerDeath(TWeakObjectPtr<class ALightSeekerPlayerState> PS, FTransform Transform)
{
	UE_LOG(LogTemp, Verbose, TEXT("ALevelGamemodeBase::OnPlayerDeath called"));
	--LivePlayerCount;
	if (LivePlayerCount == 0)
	{
		// game over
		UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	}

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
			++LivePlayerCount;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ALevelGamemodeBase::OnPlayerRevive failed"));
		}
	}
}
