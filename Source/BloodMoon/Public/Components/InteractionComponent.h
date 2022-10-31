// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AInteractableActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredNewInteractableSignature, AInteractableActor*, HoveredInteractable);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLOODMOON_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();
	
public:
	void Interact();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Server, Reliable)
	void ServerInteract(AInteractableActor* AttemptedInteractable);
	
	AInteractableActor* LineTraceForInteractableActor() const;

public:
	FOnHoveredNewInteractableSignature OnHoveredNewInteractable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractionDistance = 100.0f;

protected:
	UPROPERTY()
	AInteractableActor* CurrentHoveredInteractable;
};
