﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class UPortalManager;
class UJoinGame;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UJoinGame> JoinGameWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UFUNCTION()
	void OnJoinGameButtonClicked();

	UFUNCTION()
	void UpdateJoinGameStatusMessage(const FString& Message);
	
};
