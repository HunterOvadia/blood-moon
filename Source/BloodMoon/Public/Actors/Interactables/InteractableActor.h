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
	virtual void OnInteract(AActor* Interactee);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractBP(AActor* Interactee);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Interactable)
	UStaticMeshComponent* StaticMeshComponent;
};
