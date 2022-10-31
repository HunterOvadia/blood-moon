// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class BLOODMOON_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

public:
	virtual uint8 GetCurrentState() const PURE_VIRTUAL(AInteractableActor::GetCurrentState, return 0;);
	virtual FText GetCurrentActionText() const PURE_VIRTUAL(AInteractableActor::GetCurrentActionText, return FText(););
	virtual void OnInteract(AActor* Interactee) PURE_VIRTUAL(AInteractableActor::GetCurrentActionText, );
	const FText& GetDisplayName() const { return DisplayName; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Interactable)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Interactable)
	UStaticMeshComponent* StaticMeshComponent;
};
