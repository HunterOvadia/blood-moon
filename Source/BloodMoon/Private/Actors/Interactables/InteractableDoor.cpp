// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactables/InteractableDoor.h"

#include "Net/UnrealNetwork.h"

AInteractableDoor::AInteractableDoor()
{
	DoorState = static_cast<uint8>(EDoorState::Closed);
	RotationAxis = EAxis::Type::Z;
	OpenAngle = 90.0f;
	ClosedAngle = 0.0f;
}

FText AInteractableDoor::GetCurrentActionText() const
{
	if(IsClosed())
	{
		if(IsLocked())
		{
			return FText::FromString(TEXT("Unlock"));
		}

		return FText::FromString(TEXT("Open"));
	}

	if(IsOpen())
	{
		return FText::FromString(TEXT("Close"));
	}

	return FText();
}

void AInteractableDoor::OnInteract(AActor* Interactee)
{
	if(IsClosed())
	{
		if(IsLocked())
		{
			return;
		}
		
		OpenDoor();
	}
	else if(IsOpen())
	{
		CloseDoor();
	}
}

uint8 AInteractableDoor::GetCurrentState() const
{
	return DoorState;
}

void AInteractableDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	constexpr FDoRepLifetimeParams PushModelNone { COND_None,REPNOTIFY_Always,true };
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DoorState, PushModelNone);
}

bool AInteractableDoor::IsLocked() const
{
	return DoorState & static_cast<uint8>(EDoorState::Locked);
}

bool AInteractableDoor::IsClosed() const
{
	return DoorState & static_cast<uint8>(EDoorState::Closed);
}

bool AInteractableDoor::IsOpen() const
{
	return DoorState & static_cast<uint8>(EDoorState::Open);
}

void AInteractableDoor::Lock()
{
	DoorState |= static_cast<uint8>(EDoorState::Locked);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DoorState, this)
}

void AInteractableDoor::Unlock()
{
	DoorState &= ~static_cast<uint8>(EDoorState::Locked);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DoorState, this);
}

void AInteractableDoor::OpenDoor()
{
	if(IsLocked())
	{
		return;
	}

	SetDoorStateOpen();
	UpdateDoor();
}

void AInteractableDoor::CloseDoor()
{
	SetDoorStateClosed();
	UpdateDoor();
}

void AInteractableDoor::SetDoorStateOpen()
{
	DoorState &= ~static_cast<uint8>(EDoorState::Locked);
	DoorState &= ~static_cast<uint8>(EDoorState::Closed);
	DoorState |= static_cast<uint8>(EDoorState::Open);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DoorState, this);
}

void AInteractableDoor::SetDoorStateClosed()
{
	DoorState &= ~static_cast<uint8>(EDoorState::Open);
	DoorState |= static_cast<uint8>(EDoorState::Closed);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DoorState, this);
}

void AInteractableDoor::UpdateDoor()
{
	FRotator NewRotation;
	
	const FRotator& Current = GetActorRotation();
	const float NewRotationAngle = IsClosed() ? ClosedAngle : OpenAngle;
	switch(RotationAxis)
	{
		case EAxis::None: break;
		case EAxis::X: NewRotation = FRotator(Current.Pitch, Current.Yaw, NewRotationAngle); break;
		case EAxis::Y: NewRotation = FRotator(NewRotationAngle, Current.Yaw, Current.Roll); break;
		case EAxis::Z: NewRotation = FRotator(Current.Pitch, NewRotationAngle, Current.Roll); break;
		default: break;
	}
	
	SetActorRotation(NewRotation);
}

void AInteractableDoor::OnRep_DoorState()
{
	UpdateDoor();
}
