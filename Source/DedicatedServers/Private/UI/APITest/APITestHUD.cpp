﻿// // Copyright invi1998


#include "UI/APITest/APITestHUD.h"

#include "UI/APITest/APITestOverlay.h"

void AAPITestHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) && APITestOverlayClass)
	{
		APITestOverlay = CreateWidget<UAPITestOverlay>(PlayerController, APITestOverlayClass);
		APITestOverlay->AddToViewport();
	}
}
