// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePage.generated.h"

class UGameLoading;
class UGameSessionsManager;
class UCommonButtonBase;
class UPortalManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGamePage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_JoinGame;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSessionsManager> GameSessionsManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameLoading> GameLoadingClass;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void JoinGameButtonClicked();

	UPROPERTY()
	TObjectPtr<UGameSessionsManager> GameSessionsManager;

	UFUNCTION()
	void SetStatusMessage(const FString& StatusMessage, bool bNeedRestButton);

	UPROPERTY()
	TObjectPtr<UGameLoading> GameLoading;
	
};
