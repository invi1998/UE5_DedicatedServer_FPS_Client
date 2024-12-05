// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "HttpModule.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"

void UPortalManager::JoinGameSession()
{
	OnJoinGameSessionMessage.Broadcast(TEXT("Searching for game session..."));

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
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		if (UHTTPRequestManager::ContainErrors(JsonObject))
		{
			OnJoinGameSessionMessage.Broadcast(TEXT("Failed to find or create game session"));
			return;
		}

		UHTTPRequestManager::DumpMetaData(JsonObject);

		OnJoinGameSessionMessage.Broadcast(TEXT("Game session found or created"));
	}
	else
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("FindOrCreateGameSession_Response: Failed to deserialize JSON response"));
	}

}
