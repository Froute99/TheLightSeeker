// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/BoxComponent.h"	
#include "EnemyBase.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (Mesh)
	{
		SetRootComponent(Mesh);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	}
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBeginOverlap);
}

void ATrap::DestroySelf()
{
	for (auto Pair : EnemiesUnderEffect)
	{
		if (IsValid(Pair.Key))
		{
			Pair.Key->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(Pair.Value);
		}
	}
	Destroy();
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTime -= DeltaTime;
	if (LifeTime < 0.0f)
	{
		DestroySelf();
	}
}

void ATrap::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OtherActor);
	if (!EnemyBase) return;

	if (UAbilitySystemComponent* EnemyASC = EnemyBase->GetAbilitySystemComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Trap Activated"));

		if (TrapGameplayEffectSpecHandle.IsValid())
		{			
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = EnemyASC->ApplyGameplayEffectSpecToSelf(*TrapGameplayEffectSpecHandle.Data.Get());
			EnemiesUnderEffect.Add({ EnemyBase, ActiveGameplayEffectHandle });			
		}
	}
}

