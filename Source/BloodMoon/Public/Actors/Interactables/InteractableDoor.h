// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "GameFramework/Actor.h"
#include "InteractableDoor.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDoorState : uint8
{
	None	=	0 UMETA(Hidden),
	Locked	=	1 << 0,
	Closed	=	1 << 1,
	Open	=	1 << 2
};
ENUM_CLASS_FLAGS(EDoorState);

UCLASS()
class BLOODMOON_API AInteractableDoor : public AInteractableActor
{
	GENERATED_BODY()

public:
	AInteractableDoor();
	virtual void OnInteract(AActor* Interactee) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	bool IsLocked() const;
	bool IsClosed() const;
	bool IsOpen() const;

	void Lock();
	void Unlock();
	void OpenDoor();
	void CloseDoor();

	void SetDoorStateOpen();
	void SetDoorStateClosed();

	void UpdateDoor();
	
	UFUNCTION()
	void OnRep_DoorState();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, ReplicatedUsing=OnRep_DoorState, meta = (Bitmask, BitmaskEnum = EDoorState))
	uint8 DoorState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAxis::Type> RotationAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClosedAngle;
};
