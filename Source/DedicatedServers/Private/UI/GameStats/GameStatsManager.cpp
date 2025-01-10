// // Copyright invi1998


#include "UI/GameStats/GameStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "HTTP/HTTPRequestTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Player/DSLocalPlayerSubsystem.h"

void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& MatchStatsInput)
{
	// 通过FJsonObjectConverter将结构体转换为JsonObject
	// TSharedPtr<FJsonObject> MatchStatsJson = MakeShareable(new FJsonObject());
	// FJsonObjectConverter::UStructToJsonObject(FDSRecordMatchStatsInput::StaticStruct(), &MatchStatsInput, MatchStatsJson.ToSharedRef());

	// 将JsonObject转换为字符串
	FString MatchStatsJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &MatchStatsInput, MatchStatsJsonString);

	// GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, MatchStatsJsonString);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		const FString AccessToken = LocalPlayerSubsystem->GetAuthenticationResult().AccessToken;
		Request->SetHeader(TEXT("Authorization"), AccessToken);
	}
	
	Request->SetContentAsString(MatchStatsJsonString);

	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);

	Request->ProcessRequest();
	
}

void UGameStatsManager::RetrieveMatchStats()
{
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RetrieveMatchStats);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	const FString AccessToken = LocalPlayerSubsystem->GetAuthenticationResult().AccessToken;
	Request->SetHeader(TEXT("Authorization"), AccessToken);

	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response);

	Request->ProcessRequest();
	
}

void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats request failed"));
		return;
	}

	
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			return;
		}
	}
}

void UGameStatsManager::RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrieveMatchStatsReceived.Broadcast(FDSRetrieveMatchStatsResponse());
		return;
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			return;
		}

		FDSRetrieveMatchStatsResponse RetrieveMatchStatsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveMatchStatsResponse);
		RetrieveMatchStatsResponse.Dump();

		OnRetrieveMatchStatsReceived.Broadcast(RetrieveMatchStatsResponse);
	}
}
