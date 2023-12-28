// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Abilities/RangeEnemyProjectile.h"
#include "Characters/LightSeekerPlayerState.h"
#include "Characters/CharacterBase.h"
#include "Components/BoxComponent.h"

ARangeEnemyProjectile::ARangeEnemyProjectile()
{
	if (UBoxComponent* BoxComp = Cast<UBoxComponent>(RootComponent))
	{
		BoxComp->OnComponentBeginOverlap.Clear(); // remove original overlap function
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARangeEnemyProjectile::OnBeginOverlap_EnemyAttack);
	}
}

void ARangeEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangeEnemyProjectile::OnBeginOverlap_EnemyAttack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
	if (Player)
	{
		if (!DamageEffectSpecHandle.IsValid())
		{
			UE_LOG(Enemy, Warning, TEXT("RangeEnemyProjectile effect handle is not valid"));
			return;
		}

		ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Player->GetPlayerState());
		if (PS)
		{
			UE_LOG(Enemy, Warning, TEXT("RangeEnemyProjectile hit player - before: %f"), PS->GetHealth());
			PS->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*(DamageEffectSpecHandle.Data.Get()));
			UE_LOG(Enemy, Warning, TEXT("RangeEnemyProjectile hit player - after: %f"), PS->GetHealth());
		}
		Destroy();
	}
	else
	{
		Destroy();
	}
}