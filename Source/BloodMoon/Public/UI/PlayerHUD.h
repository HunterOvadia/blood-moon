// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/Object.h"
#include "PlayerHUD.generated.h"

class AInteractableActor;
class UPlayerInformationWidget;
class UHoveredInteractableWidget;

UCLASS()
class BLOODMOON_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	void OnHoveredNewInteractable(AInteractableActor* NewInteractable);
	
protected:
	virtual void BeginPlay() override;

	template<typename TClass>
	bool CreateAndAddWidget(const TSubclassOf<TClass>& Class, TClass*& Instance);

protected:
	// Player Information Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerInformationWidget> PlayerInformationWidgetClass;
	
	UPROPERTY(BlueprintReadWrite)
	UPlayerInformationWidget* PlayerInformationWidget;

	// Hovered Interactable Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHoveredInteractableWidget> HoveredInteractableWidgetClass;
	
	UPROPERTY(BlueprintReadWrite)
	UHoveredInteractableWidget* HoveredInteractableWidget;
};

template <typename TClass>
bool APlayerHUD::CreateAndAddWidget(const TSubclassOf<TClass>& Class, TClass*& Instance)
{
	Instance = CreateWidget<TClass>(GetWorld(), Class);
	if(IsValid(Instance))
	{
		Instance->AddToViewport();
		return true;
	}

	return false;
}
