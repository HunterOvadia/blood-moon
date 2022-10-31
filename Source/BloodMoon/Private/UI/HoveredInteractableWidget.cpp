// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HoveredInteractableWidget.h"

#include "Actors/Interactables/InteractableActor.h"

void UHoveredInteractableWidget::OnHoveredNewInteractable(AInteractableActor* NewInteractable)
{
	if(IsValid(NewInteractable))
	{
		InteractableNameText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		InteractableNameText->SetVisibility(ESlateVisibility::Collapsed);	
	}
}
