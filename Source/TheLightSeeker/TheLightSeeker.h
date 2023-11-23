// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(FDelegate_SetAbilityEnd); // Enemy Abilitly End event
DECLARE_MULTICAST_DELEGATE(FDelegate_HealthChanged); // Enemy Abilitly End event

DECLARE_LOG_CATEGORY_EXTERN(Enemy, Log, All);

#define ENEMY_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ENEMYLOG(Verbosity, Format, ...) UE_LOG(Enemy, Verbosity, TEXT("%s %s"), *ENEMY_CALLINFO, *Fstring::Printf(Format, ##__VA_ARGS__))


UENUM(BlueprintType)
enum class ELSAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	DefaultAttack	UMETA(DisplayName = "DefaultAttack"),
	// 4 1
	Ability1		UMETA(DisplayName = "Ability1"),
	// 5 2
	Ability2		UMETA(DisplayName = "Ability2"),
	// 6 3
	Ability3		UMETA(DisplayName = "Ability3"),
	// 7 4
	Ability4		UMETA(DisplayName = "Ability4"),
	// 8 Dodge
	Dodge			UMETA(DisplayName = "Sprint"),
	// 9 Jump
	Jump			UMETA(DisplayName = "Jump")
};

