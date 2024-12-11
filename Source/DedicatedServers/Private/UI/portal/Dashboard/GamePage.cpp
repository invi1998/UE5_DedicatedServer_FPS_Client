// // Copyright invi1998


#include "UI/portal/Dashboard/GamePage.h"

#include "CommonButtonBase.h"
#include "UI/GameSessions/GameSessionsManager.h"
#include "UI/portal/Dashboard/GameLoading.h"

void UGamePage::NativeConstruct()
{
	Super::NativeConstruct();

	GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	if (IsValid(GameSessionsManager) && IsValid(Button_JoinGame))
	{
		Button_JoinGame->OnClicked().AddUObject(this, &UGamePage::JoinGameButtonClicked);
		GameSessionsManager->OnJoinGameSessionMessage.AddDynamic(this, &UGamePage::SetStatusMessage);
	}
	
}

void UGamePage::JoinGameButtonClicked()
{
	Button_JoinGame->SetIsEnabled(false);

	// 显示游戏加载页面
	GameLoading = CreateWidget<UGameLoading>(GetOwningPlayer(), GameLoadingClass);
	if (IsValid(GameLoading))
	{
		GameLoading->AddToViewport();
	}
}

void UGamePage::SetStatusMessage(const FString& StatusMessage, bool bNeedRestButton)
{
	if (bNeedRestButton)
	{
		Button_JoinGame->SetIsEnabled(true);

		if (IsValid(GameLoading))
		{
			GameLoading->RemoveFromParent();

			return;
		}
	}

	GameLoading->AddLoadingText(StatusMessage);
	
}
