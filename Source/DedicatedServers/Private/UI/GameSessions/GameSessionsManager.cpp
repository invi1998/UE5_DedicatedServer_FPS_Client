// // Copyright invi1998


#include "UI/GameSessions/GameSessionsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "HTTP/HTTPRequestTypes.h"
#include "Player/DSLocalPlayerSubsystem.h"

void UGameSessionsManager::JoinGameSession(bool bRetry)
{
	OnJoinGameSessionMessage.Broadcast(bRetry ? HTTPStatusMessages::GameSessionActivating : HTTPStatusMessages::SearchingForGameSession, false);

	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		const FString AccessToken = LocalPlayerSubsystem->GetAuthenticationResult().AccessToken;
		Request->SetHeader(TEXT("Authorization"), AccessToken);
	}

	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::FindOrCreateGameSession_Response);

	Request->ProcessRequest();
}

void UGameSessionsManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
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

void UGameSessionsManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::PlayerSessionCreateFailed, true);
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return;
		}
		
		FDSPlayerSession PlayerSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSession);

		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::PlayerSessionCreateSuccess, false);

		if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
		{
			FInputModeGameOnly InputModeGameOnly;
			LocalPlayerController->SetInputMode(InputModeGameOnly);
			LocalPlayerController->SetShowMouseCursor(false);
		}
		
		// 进入游戏
		const FString IpAndPort = PlayerSession.IpAddress + FString::Printf(TEXT(":%d"), PlayerSession.Port);
		const FName Address(*IpAndPort);
		UGameplayStatics::OpenLevel(this, Address);
	}
}

FString UGameSessionsManager::GetUniquePlayerID() const
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

void UGameSessionsManager::HandleGameSessionStatus(const FString& Status, const FString& SessionId)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		TryCreatePlayerSession(SessionId, GetUniquePlayerID());
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		// 创建一个定时器，每隔一段时间检查游戏会话状态
		FTimerDelegate CreateGameSessionTimerDelegate = FTimerDelegate::CreateUObject(this, &UGameSessionsManager::JoinGameSession, true);
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

void UGameSessionsManager::TryCreatePlayerSession(const FString& GameSessionId, const FString& PlayerId)
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
	
	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::CreatePlayerSession_Response);

	Request->ProcessRequest();
}
