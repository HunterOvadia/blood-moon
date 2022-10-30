// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInformationWidget.generated.h"


class UStatDisplayWidget;

UCLASS()
class BLOODMOON_API UPlayerInformationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UStatDisplayWidget* HealthDisplayWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UStatDisplayWidget* HungerDisplayWidget;
};
