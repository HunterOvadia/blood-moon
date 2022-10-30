// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInformationWidget.h"
#include "GameFramework/HUD.h"
#include "UObject/Object.h"
#include "PlayerHUD.generated.h"

UCLASS()
class BLOODMOON_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	template<typename TClass>
	bool CreateAndAddWidget(const TSubclassOf<TClass>& Class, TClass*& Instance);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerInformationWidget> PlayerInformationWidgetClass;
	
	UPROPERTY(BlueprintReadWrite)
	UPlayerInformationWidget* PlayerInformationWidget;
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
