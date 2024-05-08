// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "LevelGamemodeBase.h"	
#include "GameFramework/GameSession.h"
#include "GameFramework/OnlineSession.h"

ALevelGamemodeBase::ALevelGamemodeBase()
{
	HasGameStarted = false;
	CurrentPlayerCount = 0;
}

void ALevelGamemodeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Log, TEXT("PreLogin from Gamemodebase called"));
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