// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/BloodMoonCharacter.h"
#include "Components/TextBlock.h"
#include "StatDisplayWidget.generated.h"

enum class EStatsType : uint8;

UCLASS()
class BLOODMOON_API UStatDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnStatUpdated(EStatsType Type, int32 NewValue);
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText Prefix;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	EStatsType BoundStat;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* StatTextBlock;
	
	UPROPERTY()
	UStatsComponent* TargetStatsComponent;
};
