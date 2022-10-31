// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HoveredInteractableWidget.generated.h"

class AInteractableActor;

UCLASS()
class BLOODMOON_API UHoveredInteractableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnHoveredNewInteractable(AInteractableActor* NewInteractable);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateText();
	
protected:
	UPROPERTY()
	AInteractableActor* CurrentInteractableActor;

	uint8 PreviousState;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InteractableNameText;
};
