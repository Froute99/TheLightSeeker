// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/EnemyController.h"
#include "CharacterBase.h"
#include "LightSeekerPlayerState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemies/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "CharacterAbilitySystemComponent.h"

AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		UBehaviorTree* BTAsset = CastChecked<AEnemyBase>(InPawn)->GetBTAsset();
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyController couldn't run behavior tree!"));
		}
		else
		{
			float AttackRange = CastChecked<AEnemyBase>(InPawn)->GetAttackRange();
			BlackboardComponent->SetValueAsFloat(FName("AttackRange"), AttackRange);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed UseBlackboard()"));
	}
}

bool AEnemyController::BindDelegate()
{
	// ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(PlayerState);
	// if (!IsValid(PS))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("PS is invalid"));
	//	return false;
	// }

	// UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AEnemyBase* E = Cast<AEnemyBase>(GetCharacter());
	if (!IsValid(E))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot get EnemyBase"));
		return false;
	}

	UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(E->GetAbilitySystemComponent());
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy ASC invalid"));
		return false;
	}

	ASC->OnReceivedDamage.AddDynamic(this, &AEnemyController::FloatingDamage);
	return true;
}
