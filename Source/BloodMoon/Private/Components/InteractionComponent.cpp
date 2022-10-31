// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/InteractionComponent.h"
#include "Actors/Interactables//InteractableActor.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::Interact()
{
	if(IsValid(CurrentHoveredInteractable))
	{
		// NOTE(HO): We will tell the server what we interacted with, but it will verify validity.
		ServerInteract(CurrentHoveredInteractable);
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if(!IsValid(Owner))
	{
		return;
	}

	if(!Owner->HasAuthority())
	{
		AInteractableActor* CurrentTraceActor = LineTraceForInteractableActor();
		if(CurrentTraceActor != CurrentHoveredInteractable)
		{
			CurrentHoveredInteractable = CurrentTraceActor;
			OnHoveredNewInteractable.Broadcast(CurrentHoveredInteractable);
		}
	}
}

void UInteractionComponent::ServerInteract_Implementation(AInteractableActor* AttemptedInteractable)
{
	if(!IsValid(AttemptedInteractable))
	{
		return;
	}

	// NOTE(HO): Ensure the client isn't cheating by tracing on the server and verifying what the client detected is the same actor.
	const AInteractableActor* LineTracedInteractable = LineTraceForInteractableActor();
	if(IsValid(AttemptedInteractable) && AttemptedInteractable == LineTracedInteractable)
	{
		AttemptedInteractable->OnInteract(GetOwner());
	}
}

AInteractableActor* UInteractionComponent::LineTraceForInteractableActor() const
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner))
	{
		return nullptr;
	}
	
	FHitResult HitResult;
	FVector Start = Owner->GetActorLocation();
	FVector End = (Start + (Owner->GetActorForwardVector() * InteractionDistance));
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic;

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	CollisionQueryParams.AddIgnoredActor(Owner);
	
	FCollisionResponseParams CollisionResponseParams = FCollisionResponseParams::DefaultResponseParam;
	if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, CollisionQueryParams, CollisionResponseParams))
	{
		AActor* HitActor = HitResult.GetActor();
		if(IsValid(HitActor))
		{
			AInteractableActor* InteractableActor = Cast<AInteractableActor>(HitActor);
			if(InteractableActor)
			{
				return InteractableActor;
			}
		}
	}

	return nullptr;
}
