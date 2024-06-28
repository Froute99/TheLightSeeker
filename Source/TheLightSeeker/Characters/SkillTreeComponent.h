// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THELIGHTSEEKER_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillTreeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<class UCharacterGameplayAbility>> AbilityList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UCharacterGameplayAbility> DodgeAbility;

	UFUNCTION(BlueprintCallable)
	bool IsAbilityAcquired(const FString& AbilityName);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void IncreaseSkillPoint();
	int GetSkillPointNum() { return SkillPoints; }

	UFUNCTION(Client, Reliable)
	void SubtractSkillPoint(int NumSkillPoint);


	UFUNCTION(BlueprintCallable)
	void ToggleWidgetC();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SkillTreeWidgetClass;

private:
	bool IsWidgetRender = false;
	class UUserWidget* SkillTreeWidgetRef;

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	int SkillPoints = 0;

};
