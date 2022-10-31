// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "Actors/Interactables/InteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HoveredInteractableWidget.h"

void APlayerHUD::OnHoveredNewInteractable(AInteractableActor* NewInteractable) const
{
	if(IsValid(HoveredInteractableWidget))
	{
		if(IsValid(NewInteractable))
		{
			FVector2D Position;
			if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), NewInteractable->GetActorLocation(), Position))
			{
				HoveredInteractableWidget->SetPositionInViewport(Position);
				HoveredInteractableWidget->OnHoveredNewInteractable(NewInteractable);
				HoveredInteractableWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		else
		{
			HoveredInteractableWidget->SetVisibility(ESlateVisibility::Collapsed);
			HoveredInteractableWidget->OnHoveredNewInteractable(nullptr);
		}
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
