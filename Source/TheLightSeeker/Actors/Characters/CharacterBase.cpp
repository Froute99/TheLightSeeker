// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"

#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"

#include "ProjectileBase.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "TheLightSeeker.h"

#include "GameAbilitySystem/CharacterGameplayAbility.h"

#include "UI/ItemWidget.h"

#include "Actors/Characters/SkillTreeComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	//SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	SkillTreeComponent = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTree"));

}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	ALightSeekerPlayerState* PS = GetPlayerState<ALightSeekerPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);
		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();
	}

}


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!ArrowActor)
	{
		FString Msg = FString::Printf(TEXT("Missing Arrow Blueprint %s. Please fill in the character's Blueprint."), *GetName());
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, *Msg);
		return;
	}
	HasItem = false;
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return ASC.Get();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* PC = CastChecked<APlayerController>(Controller);

	check(EIC && PC);


	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::EnhancedMove);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::EnhancedLook);
	EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacterBase::Jump);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ACharacterBase::CameraZoom);
	EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACharacterBase::Attack);
	EIC->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ACharacterBase::Dodge);
	EIC->BindAction(ItemAction, ETriggerEvent::Triggered, this, &ACharacterBase::UseItem);

	EIC->BindAction(Skill1Action, ETriggerEvent::Triggered, this, &ACharacterBase::Ability1);
	EIC->BindAction(Skill2Action, ETriggerEvent::Triggered, this, &ACharacterBase::Ability2);

	EIC->BindAction(ConfirmAction, ETriggerEvent::Triggered, ASC.Get(), &UCharacterAbilitySystemComponent::LocalInputConfirm);
	EIC->BindAction(CancelAction, ETriggerEvent::Triggered, ASC.Get(), &UCharacterAbilitySystemComponent::LocalInputCancel);


	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(MappingContext, 0);
}

void ACharacterBase::EnhancedMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.Y);

}

void ACharacterBase::EnhancedLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(-LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

}

void ACharacterBase::CameraZoom(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() == 0.f || !Controller) return;


	const float NewTargetArmLength = SpringArm->TargetArmLength + Value.GetMagnitude() * ZoomStep;
	SpringArm->TargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomLength, MaxZoomLength);
}

void ACharacterBase::SetCharacterLevel(float Value)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetLevel(Value);
	}
}

void ACharacterBase::SetHealth(float Value)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Value);
	}
}

void ACharacterBase::SetMaxHealth(float Value)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetMaxHealth(Value);
	}
}

int32 ACharacterBase::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
	{
		return FMath::Floor(AttributeSet->GetLevel());
	}

	return 1;
}

float ACharacterBase::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

void ACharacterBase::OnRep_PlayerState()
{
	ALightSeekerPlayerState* PS = GetPlayerState<ALightSeekerPlayerState>();

	if (PS)
	{
		InitializeStartingValues(PS);
	}
}

void ACharacterBase::InitializeStartingValues(ALightSeekerPlayerState* PS)
{
	ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSet = PS->GetAttributeSet();

	//AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	//SetHealth(GetMaxHealth());
	//SetCharacterLevel(GetCharacterLevel());


	InitializeAttributes();
}

void ACharacterBase::AddCharacterAbilities(/*TSubclassOf<UGameplayAbility>& Ability*/)
{
	//ASC->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));

	if (GetLocalRole() != ROLE_Authority || !ASC.IsValid() || ASC->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UCharacterGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, -1, this));
	}

	ASC->CharacterAbilitiesGiven = true;

}

void ACharacterBase::InitializeAttributes()
{
	if (!ASC.IsValid()) return;


	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
	}

	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();
}

void ACharacterBase::AddStartupEffects()
{

}

void ACharacterBase::Attack(const FInputActionValue& Value)
{
	//GetMesh()->PlayAnimation(AttackMontage, false);
	bool Succeed = ASC->TryActivateAbilitiesByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.DefaultAttack")).GetSingleTagContainer());
	if (Succeed)
	{
		UE_LOG(LogTemp, Log, TEXT("Activate Default Attack"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Can't activate Default Attack"));
	}


	//FVector Location = GetActorLocation();
	//FVector LocationOffset{ 0,0,30.f };

	//FRotator Rotation = GetActorRotation();
	//FRotator RotationOffset{ 0,90.f,0 };

	//FActorSpawnParameters Parameter{};
	//Parameter.Instigator = this;
	//AProjectileBase* Arrow = GetWorld()->SpawnActor<AProjectileBase>(ArrowClass, Location + LocationOffset, Rotation, Parameter);
	//
	//if (Arrow)
	//{
	//	FVector LaunchDirection = GetActorForwardVector();
	//	Arrow->FireInDirection(LaunchDirection);
	//}

}

void ACharacterBase::Dodge(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Dodge"));

	GetMesh()->PlayAnimation(DodgeMontage, false);

}

void ACharacterBase::OnPickupItem(TSubclassOf<class UCharacterGameplayAbility> ItemAbility, UTexture2D* Icon)
{
	UCharacterGameplayAbility* Ability = ItemAbility.GetDefaultObject();

	if (!Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Cast Item Ability"));
		return;
	}

	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(FGameplayAbilitySpec(ItemAbility, 1, -1, this));
	
	if (!Handle.IsValid()) UE_LOG(LogTemp, Warning, TEXT("Failed to Grant ItemAbility"));

	// if Ability is used on granting, remove it immediately
	if (!Ability->ActivateAbilityOnGranted)
	{
		UE_LOG(LogTemp, Log, TEXT("Got Item"));
		HasItem = true;

		// Set Image
		if (ItemWidget)
			ItemWidget->SetIcon(Icon);
		else
			UE_LOG(LogTemp, Error, TEXT("ItemWidget not found"));
	}
	else
	{
		// for potion - clear ability so that ability cannot be used by player
		UE_LOG(LogTemp, Log, TEXT("Clear Item successfully"));
		ASC->ClearAbility(Handle);
	}
}

void ACharacterBase::UseItem()
{
	if (!HasItem || GetLocalRole() != ROLE_Authority) return;

	bool Succeed = ASC->TryActivateAbilitiesByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Item")).GetSingleTagContainer());
	if (Succeed)
	{
		HasItem = false;
		ItemWidget->ClearIcon();
		UE_LOG(LogTemp, Log, TEXT("Activated Item"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Could not activate Item"));
	}
}

void ACharacterBase::Ability1()
{
	//if (IsValid(GameplayAbility1->Get()))
	//{
	//	if (ASC->TryActivateAbilityByClass(GameplayAbility1->Get()))
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Ability1 Activated"));
	//	}
	//}
	



	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[0]);

	//ASC->TryActivateAbility(GameplayAbility1);

	//ASC->GetActivatableAbilities().Find()

}

void ACharacterBase::Ability2()
{
	//ASC->TryActivateAbility(GameplayAbility2);
	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[1]);

}

