// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UPortalManager::JoinGameSession()
{
	OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SearchingForGameSession, false);

	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::FindOrCreateGameSession_Response);

	Request->ProcessRequest();
}

void UPortalManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && !ContainErrors(JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return;
		}
		
		FDSGameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession, 0, 0);

		const FString GameSessionId = GameSession.GameSessionId;
		const FString GameSessionStatus = GameSession.Status;
		HandleGameSessionStatus(GameSessionStatus, GameSessionId);
		
	}

}

FString UPortalManager::GetUniquePlayerID() const
{
	FString PlayerUniqueId{"Player_"};
	if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
	{
		// GetUniqueId 和 GetUniqueID，前者
		APlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(LocalPlayerState) && LocalPlayerState->GetUniqueId().IsValid())
		{
			PlayerUniqueId  += FString::FromInt(LocalPlayerState->GetUniqueID());
		}
	}

	return PlayerUniqueId;
}

void UPortalManager::HandleGameSessionStatus(const FString& Status, const FString& SessionId)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::FindGameSessionSuccess, false);
		TryCreatePlayerSession(SessionId, GetUniquePlayerID());
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::GameSessionActivating, false);

		// 创建一个定时器，每隔一段时间检查游戏会话状态
		FTimerDelegate CreateGameSessionTimerDelegate = FTimerDelegate::CreateUObject(this, &UPortalManager::JoinGameSession);
		if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(CreateGameSessionTimerHandle, CreateGameSessionTimerDelegate, 1.f, false);
		}
	}
	else
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
}

void UPortalManager::TryCreatePlayerSession(const FString& GameSessionId, const FString& PlayerId)
{
	
}
