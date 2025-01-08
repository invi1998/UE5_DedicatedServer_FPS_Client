// // Copyright invi1998


#include "UI/GameStats/GameStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "HTTP/HTTPRequestTypes.h"
#include "Interfaces/IHttpRequest.h"

void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& MatchStatsInput)
{
	// 通过FJsonObjectConverter将结构体转换为JsonObject
	TSharedPtr<FJsonObject> MatchStatsJson = MakeShareable(new FJsonObject());
	FJsonObjectConverter::UStructToJsonObject(FDSRecordMatchStatsInput::StaticStruct(), &MatchStatsInput, MatchStatsJson.ToSharedRef());

	// 将JsonObject转换为字符串
	FString MatchStatsJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &MatchStatsInput, MatchStatsJsonString);

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, MatchStatsJsonString);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(MatchStatsJsonString);

	// Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);

	Request->ProcessRequest();
	
}
