// Copyright Epic Games, Inc. All Rights Reserved.
#include "Character/BloodMoonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/StatsComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ABloodMoonCharacter::ABloodMoonCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->InteractionDistance = 100.0f;
	
	TurnRateGamepad = 50.f;
}

void ABloodMoonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		StatsComponent->AddStat(EStatsType::Health, 100);
		StatsComponent->AddStat(EStatsType::Hunger, 100);
		HungerTickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ABloodMoonCharacter::TickHunger), 10.0f);
	}
	else
	{
		if(const APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if(AHUD* HUD = PC->GetHUD())
			{
				PlayerHUD = Cast<APlayerHUD>(HUD);
			}
		}
		
		if(IsValid(InteractionComponent))
		{
			InteractionComponent->OnHoveredNewInteractable.AddDynamic(this, &ThisClass::OnHoveredNewInteractable);
		}
	}
}

void ABloodMoonCharacter::BeginDestroy()
{
	if(HasAuthority())
	{
		if(HungerTickHandle.IsValid())
		{
			FTSTicker::GetCoreTicker().RemoveTicker(HungerTickHandle);
		}
	}
	else
	{
		if(IsValid(InteractionComponent))
		{
			InteractionComponent->OnHoveredNewInteractable.RemoveDynamic(this, &ThisClass::ABloodMoonCharacter::OnHoveredNewInteractable);
		}
	}

	Super::BeginDestroy();
}

void ABloodMoonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABloodMoonCharacter::OnInteract);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABloodMoonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABloodMoonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ABloodMoonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ABloodMoonCharacter::LookUpAtRate);
}


bool ABloodMoonCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	check(Channel);
	check(Bunch);
	check(RepFlags);

	bool bWroteSomething = false;
	for(UActorComponent* ActorComponent : ReplicatedComponents)
	{
		UReplicationKeyActorComponent* RepKeyActorComponent = Cast<UReplicationKeyActorComponent>(ActorComponent);
		if(RepKeyActorComponent!= nullptr)
		{
			if(Channel->KeyNeedsToReplicate(RepKeyActorComponent->GetUniqueID(), RepKeyActorComponent->GetReplicationKey()))
			{
				bWroteSomething |= RepKeyActorComponent->ReplicateSubobjects(Channel, Bunch, RepFlags);
				bWroteSomething |= Channel->ReplicateSubobject(RepKeyActorComponent, *Bunch, *RepFlags);
			}
		}
		else
		{
			bWroteSomething |= ActorComponent->ReplicateSubobjects(Channel, Bunch, RepFlags);
			bWroteSomething |= Channel->ReplicateSubobject(ActorComponent, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void ABloodMoonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABloodMoonCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ABloodMoonCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool ABloodMoonCharacter::TickHunger(float DeltaTime)
{
	if(!HasAuthority())
	{
		return true;
	}
	
	if(IsValid(StatsComponent))
	{
		StatsComponent->UpdateStat(EStatsType::Hunger, -1);
	}
	
	return true;
}

void ABloodMoonCharacter::OnInteract()
{
	if(IsValid(InteractionComponent))
	{
		InteractionComponent->Interact();
	}
}

void ABloodMoonCharacter::OnHoveredNewInteractable(AInteractableActor* NewInteractable)
{
	if(IsValid(PlayerHUD))
	{
		PlayerHUD->OnHoveredNewInteractable(NewInteractable);
	}
}

void ABloodMoonCharacter::MoveForward(float Value)
{
	if (IsValid(Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABloodMoonCharacter::MoveRight(float Value)
{
	if (IsValid(Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
