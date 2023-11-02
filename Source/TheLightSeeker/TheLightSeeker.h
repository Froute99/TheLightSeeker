// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(FDelegate_SetAbilityEnd); // Enemy Abilitly End event

DECLARE_LOG_CATEGORY_EXTERN(Enemy, Log, All);

#define ENEMY_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ENEMYLOG(Verbosity, Format, ...) UE_LOG(Enemy, Verbosity, TEXT("%s %s"), *ENEMY_CALLINFO, *Fstring::Printf(Format, ##__VA_ARGS__))