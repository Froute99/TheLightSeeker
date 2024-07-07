// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Characters/CharacterBase.h"
#include "Characters/LightSeekerPlayerState.h"
#include "Characters/SkillTreeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

#include "Projectiles/ProjectileBase.h"
#include "CharacterAttributeSet.h"
#include "CharacterAbilitySystemComponent.h"
#include "Abilities/CharacterGameplayAbility.h"
#include "Abilities/CharacterAnimAbility.h"

#include "UI/ItemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "TheLightSeeker.h"

#include "Kismet/KismetSystemLibrary.h"

#include "PlayerHUD.h"
#include "PlayerHealthBarWidget.h"
#include "UI/EnemyHPBarWidget.h"

#include "LevelGamemodeBase.h"
#include "Tombstone.h"

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
	// SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	// SkillTreeComponent = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTree"));

	ConstructorHelpers::FObjectFinder<USoundCue> ItemPickupSoundCueFinder(TEXT("/Script/Engine.SoundCue'/Game/SoundCollection/UI/SFX_ItemPickup.SFX_ItemPickup'"));
	if (ItemPickupSoundCueFinder.Succeeded())
	{
		ItemPickupSound = ItemPickupSoundCueFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> ItemUseFailureCueFinder(TEXT("/Script/Engine.SoundCue'/Game/SoundCollection/UI/SFX_ItemUseFailure.SFX_ItemUseFailure'"));
	if (ItemUseFailureCueFinder.Succeeded())
	{
		ItemUseFailureSound = ItemUseFailureCueFinder.Object;
	}

	CollisionEnabled = GetMesh()->GetCollisionEnabled();
	GravityScale = GetCharacterMovement()->GravityScale;
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

		SkillTreeComponent = PS->SkillTreeComponent;
		// Binding event
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HasItem = false;
	IsDead = false;
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
	APlayerController*		 PC = CastChecked<APlayerController>(Controller);

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
	EIC->BindAction(Skill3Action, ETriggerEvent::Triggered, this, &ACharacterBase::Ability3);
	EIC->BindAction(Skill4Action, ETriggerEvent::Triggered, this, &ACharacterBase::Ability4);

	EIC->BindAction(ConfirmAction, ETriggerEvent::Triggered, ASC.Get(), &UCharacterAbilitySystemComponent::LocalInputConfirm);
	EIC->BindAction(CancelAction, ETriggerEvent::Triggered, ASC.Get(), &UCharacterAbilitySystemComponent::LocalInputCancel);

	// temp: triggered. holding으로 변경해야함
	EIC->BindAction(ReviveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Revive, true);
	EIC->BindAction(ReviveAction, ETriggerEvent::Completed, this, &ACharacterBase::Revive, false);

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
	if (Value.GetMagnitude() == 0.f || !Controller)
		return;

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
	UE_LOG(LogTemp, Log, TEXT("Set Health"));
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
		UE_LOG(LogTemp, Log, TEXT("HP: %f"), AttributeSet->GetHealth());
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

	InitializeAttributes();
}

void ACharacterBase::AddCharacterAbilities()
{
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
	if (!ASC.IsValid())
		return;

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

	// GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();
}

void ACharacterBase::AddStartupEffects()
{
}

void ACharacterBase::Attack()
{
	if (IsDoingTargeting)
	{
		ASC->LocalInputConfirm();
		return;
	}

	bool Succeed = ASC->TryActivateAbilitiesByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.BasicAttack")).GetSingleTagContainer());
	if (Succeed)
	{
		UE_LOG(LogTemp, Log, TEXT("Activate Default Attack"));
	}
}

void ACharacterBase::OnPickupItem(TSubclassOf<class UCharacterGameplayAbility> ItemAbility, UTexture2D* Icon)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed OnPickupItem"));
		return;
	}
	else if (!ASC.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed OnPickupItem2"));
		return;
	}
	else if (!ASC.Get()->CharacterAbilitiesGiven)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed OnPickupItem3"));
		return;
	}

	UCharacterGameplayAbility* Ability = ItemAbility.GetDefaultObject();

	if (!Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Cast Item Ability"));
		return;
	}
	UGameplayStatics::PlaySound2D(GetWorld(), ItemPickupSound);

	// for potion - prevent removing current item
	if (Ability->ActivateAbilityOnGranted)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(ItemAbility, 1, -1, this));
		return;
	}

	// Remove original item ability
	ASC->ClearAbility(ItemAbilityHandle);

	// grant current item's ability
	ItemAbilityHandle = ASC->GiveAbility(FGameplayAbilitySpec(ItemAbility, 1, -1, this));

	if (!ItemAbilityHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Grant ItemAbility"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Got Item"));
	HasItem = true;

	Client_UpdateItemUI(Icon);
}

void ACharacterBase::UseItem()
{
	UE_LOG(LogTemp, Log, TEXT("UseItem Called"));

	if (GetLocalRole() != ROLE_Authority)
	{
		Server_UseItem();
		return;
	}

	if (!HasItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried using item without actual item"));
		Client_OnUseItemFailed();
		return;
	}

	if (!ItemAbilityHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried using item with invalid spechandle"));
		return;
	}

	bool Succeed = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Item"))));
	if (Succeed)
	{
		HasItem = false;

		// remove item ability from player
		// ASC->ClearAbility(ItemAbilityHandle);
		ItemAbilityHandle = FGameplayAbilitySpecHandle();

		Client_UpdateItemUI(nullptr);

		UE_LOG(LogTemp, Log, TEXT("Activated Item"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Could not activate Item"));
	}
}

void ACharacterBase::Server_UseItem_Implementation()
{
	UseItem();
}

void ACharacterBase::Client_UpdateItemUI_Implementation(UTexture2D* Texture)
{
	if (Texture)
	{
		ItemWidget->SetIcon(Texture);
	}
	else
	{
		ItemWidget->ClearIcon();
	}
}

void ACharacterBase::Client_OnUseItemFailed_Implementation()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ItemUseFailureSound);
}

void ACharacterBase::UpdateHealthBar()
{
	UE_LOG(LogTemp, Log, TEXT("UpdateHealthBar"));
	if (!IsValid(HealthBar))
	{
		UE_LOG(LogTemp, Log, TEXT("HealthBar invalid"));
		return;
	}
	HealthBar->SetHealth(GetHealth());
}

void ACharacterBase::InitializeBossHealthBar(float MaxHealth)
{
	if (BossHPWidget)
	{
		BossHPWidget->SetMaxHealth(MaxHealth);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("InitializeBossHealthBar: Boss HealthBar invalid"));
	}
}

void ACharacterBase::UpdateBossHealthBar(float CurrentHealth)
{
	if (BossHPWidget)
	{
		BossHPWidget->SetHealth(CurrentHealth);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateBossHealthBar: Boss HealthBar invalid"));
	}
}

void ACharacterBase::Client_DisplayGameOverUI_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Client: Game over"));
	GameOverDelegateHandle.Broadcast();
}

void ACharacterBase::IncreaseSkillPoint_Implementation()
{
	SkillTreeComponent->IncreaseSkillPoint();
}

int ACharacterBase::GetSkillPointNum()
{
	return SkillTreeComponent->GetSkillPointNum();
}

void ACharacterBase::GrantAbility_Implementation(TSubclassOf<UGameplayAbility> AbilityClass)
{
	UCharacterAnimAbility* CharacterAbility = Cast<UCharacterAnimAbility>(AbilityClass.GetDefaultObject());
	if (!IsValid(CharacterAbility))
		return;

	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 0, 0); // Ignore level, inputID arguments
	ASC->GiveAbility(AbilitySpec);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(SkillTreeComponent->GetSkillPointNum()), true, false, FColor::Red);

	SkillTreeComponent->SubtractSkillPoint(CharacterAbility->NumRequiredSkillPoint);
}

void ACharacterBase::RepSkillPointSubtract_Implementation(UCharacterAnimAbility* Ability, int SkillPoint)
{
	SkillTreeComponent->SubtractSkillPoint(Ability->NumRequiredSkillPoint);
}

void ACharacterBase::UseAbility(int Index)
{
	if (Index <= 0 || Index > MaxAbilityNum)
		return;

	if (SkillTreeComponent->AbilityList.Num() < Index)
	{
		FString Msg = "You didn't granted Ability";
		Msg.AppendInt(Index);
		UKismetSystemLibrary::PrintString(GetWorld(), Msg, true, false, FColor::Red);
		return;
	}
	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[Index - 1]);
}

void ACharacterBase::Ability1()
{
	if (SkillTreeComponent->AbilityList.Num() < 1)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("You didn't granted Ability1"), true, false, FColor::Red);
		return;
	}

	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[0]);
}

void ACharacterBase::Ability2()
{
	if (SkillTreeComponent->AbilityList.Num() < 2)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("You didn't granted Ability2"), true, false, FColor::Red);
		return;
	}

	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[1]);
}

void ACharacterBase::Ability3()
{
	if (SkillTreeComponent->AbilityList.Num() < 3)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("You didn't granted Ability3"), true, false, FColor::Red);
		return;
	}

	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[2]);
}

void ACharacterBase::Ability4()
{
	if (SkillTreeComponent->AbilityList.Num() < 4)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("You didn't granted Ability4"), true, false, FColor::Red);
		return;
	}

	ASC->TryActivateAbilityByClass(SkillTreeComponent->AbilityList[3]);
}

void ACharacterBase::Dodge()
{
	ASC->TryActivateAbilityByClass(SkillTreeComponent->DodgeAbility);
}

void ACharacterBase::Die_Implementation()
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString("You died"), true, false, FColor::Red);
	UE_LOG(LogTemp, Log, TEXT("Character Die called"));

	IsDead = true;

	// Call event in Game Mode, spawn grave
	// spawn UI
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopActiveMovement();

	DisableInput(Cast<APlayerController>(GetController()));
	ToggleReviveStatus(nullptr, false);
}

void ACharacterBase::ToggleReviveStatus(TWeakObjectPtr<class ATombstone> TombstoneActor, bool CanRevive)
{
	Tombstone = TombstoneActor;
	CanRevivePlayer = CanRevive;

	if (ReviveInstructionWidget)
	{
		ReviveInstructionWidget->SetVisibility(CanRevive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (!CanRevive)
	{
		Revive(false); // cancle revive
	}
}

void ACharacterBase::Revive(bool IsTriggered)
{
	if (CanRevivePlayer)
	{
		if (IsTriggered)
		{
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]() {
				if (this)
				{
					this->Server_Revive();
					ReviveTriggeredDelegateHandle.Broadcast(false);
				}
			});

			GetWorld()->GetTimerManager().SetTimer(ReviveCallTimerHandle, TimerDelegate, TimerForRevive, false);
			ReviveTriggeredDelegateHandle.Broadcast(true);
			Server_TriggerReviveVFX(true);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ReviveCallTimerHandle);
			ReviveTriggeredDelegateHandle.Broadcast(false);
			Server_TriggerReviveVFX(false);

		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ReviveCallTimerHandle);
		ReviveTriggeredDelegateHandle.Broadcast(false);
		Server_TriggerReviveVFX(false);
	}
}

void ACharacterBase::Server_Revive_Implementation()
{
	if (ALevelGamemodeBase* GameMode = Cast<ALevelGamemodeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnPlayerRevive();
	}
}

void ACharacterBase::Server_TriggerReviveVFX_Implementation(bool IsTriggered)
{
	if (Tombstone.Get())
	{
		Tombstone.Get()->Server_ToggleVFX(IsTriggered);
	}
}

void ACharacterBase::Multicast_OnRevived_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Character OnRevived called"));
	InitializeAttributes();

	IsDead = false;
	this->GetMesh()->SetVisibility(false);
	ToggleReviveStatus(nullptr, false);


	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]() {
		this->EnableInput(Cast<APlayerController>(GetController()));

		this->GetMesh()->SetCollisionEnabled(CollisionEnabled);
		this->GetCharacterMovement()->GravityScale = GravityScale;
		this->GetMesh()->SetVisibility(true);
	});

	FTimerHandle   TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);

}