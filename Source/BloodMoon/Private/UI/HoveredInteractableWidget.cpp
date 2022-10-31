// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/HoveredInteractableWidget.h"
#include "Actors/Interactables/InteractableActor.h"

#define LOCTEXT_NAMESPACE "UHoveredInteractableWidget"

void UHoveredInteractableWidget::OnHoveredNewInteractable(AInteractableActor* NewInteractable)
{
	CurrentInteractableActor = NewInteractable;
	if(IsValid(CurrentInteractableActor))
	{
		UpdateText();
	}
	else
	{
		PreviousState = 0;
	}
}

void UHoveredInteractableWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(IsValid(CurrentInteractableActor))
	{
		if(CurrentInteractableActor->GetCurrentState() != PreviousState)
		{
			UpdateText();
			PreviousState = CurrentInteractableActor->GetCurrentState();
		}
	}
}

void UHoveredInteractableWidget::UpdateText()
{
	if(IsValid(CurrentInteractableActor))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Action"), CurrentInteractableActor->GetCurrentActionText());
		Args.Add(TEXT("Name"), CurrentInteractableActor->GetDisplayName());
		InteractableNameText->SetText(FText::Format(LOCTEXT("HoveredInteractableText", "{Action} {Name}"), Args));
	}
}

#undef LOCTEXT_NAMESPACE
