﻿// // Copyright invi1998


#include "UI/portal/PortalHUD.h"
#include "UI/portal/SignIn/SignInOverlay.h"
#include "UI/portal/Dashboard/DashboardOverlay.h"

#include "Blueprint/UserWidget.h"

void APortalHUD::OnSignIn()
{
	APlayerController* OwnerPlayerController = GetOwningPlayerController();

	if (IsValid(SignInOverlay))
	{
		SignInOverlay->RemoveFromParent();
	}
	
	DashboardOverlay = CreateWidget<UDashboardOverlay>(OwnerPlayerController, DashboardOverlayClass);
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->AddToViewport();
	}
}

void APortalHUD::OnSignOut()
{
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->RemoveFromParent();
	}
	
	SignInOverlay = CreateWidget<USignInOverlay>(GetOwningPlayerController(), SignOverlayClass);
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->AddToViewport();
	}
	
}

void APortalHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwnerPlayerController = GetOwningPlayerController();
	SignInOverlay = CreateWidget<USignInOverlay>(OwnerPlayerController, SignOverlayClass);
	
	// rider 注释代码的快捷键是Ctrl+K+C，取消注释的快捷键是Ctrl+K+U
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->AddToViewport();
	}

	

	FInputModeGameAndUI InputMode;
	OwnerPlayerController->SetInputMode(InputMode);
	OwnerPlayerController->SetShowMouseCursor(true);
}
