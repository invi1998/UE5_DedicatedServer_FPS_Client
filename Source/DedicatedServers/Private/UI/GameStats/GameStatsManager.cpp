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
	
	Request->SetContentAsString(MatchStatsJsonString);

	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);

	Request->ProcessRequest();
	
}

void UGameStatsManager::RetrieveMatchStats()
{
	RetrieveMatchStatsStatusMessageDelegate.Broadcast(TEXT("正在获取比赛统计数据..."), false);
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RetrieveMatchStats);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	const FString AccessToken = LocalPlayerSubsystem->GetAuthenticationResult().AccessToken;
	const TMap<FString, FString> Params = {
		{TEXT("accessToken"), AccessToken}
	};
	Request->SetContentAsString(SerializeJsonContent(Params));

	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response);

	Request->ProcessRequest();
	
}

void UGameStatsManager::UpdateLeaderboard(const TArray<FString>& WinnerPlayerNames)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::UpdateLeaderboard);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TSharedPtr<FJsonObject> LeaderboardJson = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> WinnerPlayerNamesJsonArray;
	for (const FString& WinnerPlayerName : WinnerPlayerNames)
	{
		TSharedPtr<FJsonValue> WinnerPlayerNameJson = MakeShareable(new FJsonValueString(WinnerPlayerName));
		WinnerPlayerNamesJsonArray.Add(WinnerPlayerNameJson);
	}
	LeaderboardJson->SetArrayField(TEXT("playerIds"), WinnerPlayerNamesJsonArray);
	FString Content;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(LeaderboardJson.ToSharedRef(), JsonWriter);
	Request->SetContentAsString(Content);

	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::UpdateLeaderboard_Response);

	Request->ProcessRequest();
}

void UGameStatsManager::RetrieveLeaderboard()
{
	RetrieveLeaderboardStatusMessageDelegate.Broadcast(TEXT("正在获取排行榜数据..."), false);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveLeaderboard_Response);

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
		RetrieveMatchStatsStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
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

		RetrieveMatchStatsStatusMessageDelegate.Broadcast(TEXT(""), false);
		OnRetrieveMatchStatsReceived.Broadcast(RetrieveMatchStatsResponse);
		
	}
}

void UGameStatsManager::UpdateLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnUpdateLeaderboardComplete.Broadcast();
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

void UGameStatsManager::RetrieveLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		RetrieveLeaderboardStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		OnRetrieveLeaderboardReceived.Broadcast(FDSLeaderboard());
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

		FDSLeaderboard RetrieveLeaderboardResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveLeaderboardResponse);

		RetrieveLeaderboardStatusMessageDelegate.Broadcast(TEXT(""), false);
		OnRetrieveLeaderboardReceived.Broadcast(RetrieveLeaderboardResponse);
	}
}
