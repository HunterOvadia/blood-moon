﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	ensure(CreateAndAddWidget<UPlayerInformationWidget>(PlayerInformationWidgetClass, PlayerInformationWidget));
}
