// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UPortalManager::JoinGameSession(bool bRetry)
{
	OnJoinGameSessionMessage.Broadcast(bRetry ? HTTPStatusMessages::GameSessionActivating : HTTPStatusMessages::SearchingForGameSession, false);

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

void UPortalManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::PlayerSessionCreateFailed, true);
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
		
		FDSPlayerSession PlayerSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSession, 0, 0);

		PlayerSession.Dump();

		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::PlayerSessionCreateSuccess, false);
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
		TryCreatePlayerSession(SessionId, GetUniquePlayerID());
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		// 创建一个定时器，每隔一段时间检查游戏会话状态
		FTimerDelegate CreateGameSessionTimerDelegate = FTimerDelegate::CreateUObject(this, &UPortalManager::JoinGameSession, true);
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
	OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::FindGameSessionSuccess, false);
	
	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameSessionsAPI::CreatePlayerSession);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 填充API请求参数
	TMap<FString, FString> Params = {
		{TEXT("gameSessionId"), GameSessionId},
		{TEXT("playerId"), PlayerId}
	};
	const FString ContentString = SerializeJsonContent(Params);
	Request->SetContentAsString(ContentString);
	
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::CreatePlayerSession_Response);

	Request->ProcessRequest();
}
