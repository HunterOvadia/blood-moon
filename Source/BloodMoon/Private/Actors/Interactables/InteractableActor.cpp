// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactables/InteractableActor.h"

AInteractableActor::AInteractableActor()
{
	SetReplicates(true);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
}

void AInteractableActor::OnInteract(AActor* Interactee)
{
	OnInteractBP(Interactee);
}

