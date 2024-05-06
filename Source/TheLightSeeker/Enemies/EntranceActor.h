// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EntranceActor.generated.h"

/**
 *	An Actor class for entrance of the field
 */
UCLASS()
class THELIGHTSEEKER_API AEntranceActor : public AStaticMeshActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEntranceActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class UCharacterAbilitySystemComponent* ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	class UCharacterAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Entrance")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	virtual void InitializeAttributes();

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* HPBar;

	FDelegateHandle HealthChangedDelegateHandle;

	float GetHealth() const;

	bool  IsAlive() const;
	bool IsDead;

	void  OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnDied();

	UPROPERTY(EditAnywhere, Category = "Entrance")
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditDefaultsOnly, Category = "Entrance")
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Entrance")
	class USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Entrance")
	class USoundBase* DeathSound;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySound(USoundBase* SoundToPlay);

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_BreakGeometry();
};
