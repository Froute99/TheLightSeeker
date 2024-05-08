// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelGamemodeBase.generated.h"

/**
 *	A Gamemode Base class for playable level. Impelemented for blocking player when game starts
 */
UCLASS()
class THELIGHTSEEKER_API ALevelGamemodeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALevelGamemodeBase();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable)
	void SetGameStart();

protected:
	UPROPERTY(EditAnywhere)
	int32 MaxPlayerCount;

	int32 CurrentPlayerCount;

	bool HasGameStarted;
};
