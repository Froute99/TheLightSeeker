// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/SkillTreeComponent.h"

#include "CharacterGameplayAbility.h"

// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USkillTreeComponent::IsAbilityAcquired(const FString& AbilityName)
{
	for (const auto& A : AbilityList)
	{
		const FString& Prerequisite = Cast<UCharacterGameplayAbility>(A.GetDefaultObject())->AbilityName;

		if (Prerequisite.Equals(AbilityName, ESearchCase::IgnoreCase))
		{
			return true;
		}
	}

	return false;
}

