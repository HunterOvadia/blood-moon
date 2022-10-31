// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "UI/HoveredInteractableWidget.h"

void APlayerHUD::OnHoveredNewInteractable(AInteractableActor* NewInteractable)
{
	if(IsValid(HoveredInteractableWidget))
	{
		HoveredInteractableWidget->OnHoveredNewInteractable(NewInteractable);
	}
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// Player Information Widget
	ensure(CreateAndAddWidget<UPlayerInformationWidget>(PlayerInformationWidgetClass, PlayerInformationWidget));

	// Hovered Interactable Widget
	ensure(CreateAndAddWidget<UHoveredInteractableWidget>(HoveredInteractableWidgetClass, HoveredInteractableWidget));
	OnHoveredNewInteractable(nullptr);
}
