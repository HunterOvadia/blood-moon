// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/PlayerHUD.h"
#include "BloodMoonCharacter.generated.h"

class AInteractableActor;
class UInteractionComponent;

UCLASS(config=Game)
class ABloodMoonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABloodMoonCharacter();
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UStatsComponent* GetStatsComponent() const { return StatsComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void Tick(float DeltaSeconds) override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

private:
	UFUNCTION()
	bool TickHunger(float DeltaTime);
	void OnInteract();

	UFUNCTION()
	void OnHoveredNewInteractable(AInteractableActor* NewInteractable);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BloodMoon, meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BloodMoon, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

	UPROPERTY()
	APlayerHUD* PlayerHUD;
	
	FTSTicker::FDelegateHandle HungerTickHandle;
};

