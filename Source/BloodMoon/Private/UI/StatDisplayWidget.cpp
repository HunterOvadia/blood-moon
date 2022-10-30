// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatDisplayWidget.h"
#include "Character/BloodMoonCharacter.h"
#include "Components/StatsComponent.h"

#define LOCTEXT_NAMESPACE "UStatDisplayWidget"

void UStatDisplayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TargetStatsComponent = Cast<ABloodMoonCharacter>(GetOwningPlayerPawn())->GetStatsComponent();
}

void UStatDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(TargetStatsComponent))
	{
		TargetStatsComponent->OnStatChanged.AddDynamic(this, &UStatDisplayWidget::OnStatUpdated);

		const int32 Value = TargetStatsComponent->GetStatValue(BoundStat);
		OnStatUpdated(BoundStat, Value);
	}
}

void UStatDisplayWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(IsValid(TargetStatsComponent))
	{
		TargetStatsComponent->OnStatChanged.RemoveDynamic(this, &UStatDisplayWidget::OnStatUpdated);
	}
}

void UStatDisplayWidget::OnStatUpdated(EStatsType Type, int32 NewValue)
{
	if(Type != BoundStat)
	{
		return;
	}

	if(IsValid(StatTextBlock))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Prefix"), Prefix);
		Args.Add(TEXT("Value"), FText::AsNumber(NewValue));
		StatTextBlock->SetText(FText::Format(LOCTEXT("StatDisplayWidgetValue", "{Prefix} {Value}"), Args));
	}
}

#undef LOCTEXT_NAMESPACE