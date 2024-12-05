// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
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
		DumpMetaData(JsonObject);
		if (ContainErrors(JsonObject))
		{
			OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return;
		}
		
		FDSGameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession, 0, 0);
		GameSession.Dump();

		OnJoinGameSessionMessage.Broadcast(HTTPStatusMessages::FindGameSessionSuccess, false);
		
	}

}
