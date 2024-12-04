// // Copyright invi1998


#include "UI/APITest/APITestManger.h"

#include "HttpModule.h"
#include "Data/API/APIData.h"
#include "Interfaces/IHttpResponse.h"

void UAPITestManger::ListFleetsButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ListFleetsButtonClicked"));

	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::GameSessionsAPI::ListFleets);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	// 绑定回调函数
	Request->OnProcessRequestComplete().BindUObject(this, &UAPITestManger::ListFleets_Response);

	// 发送请求
	Request->ProcessRequest();
}

void UAPITestManger::ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// 创建一个JsonReader
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// 创建一个空的Json对象
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	// 将JSON字符串反序列化为JSON对象
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		if (JsonObject->HasField(TEXT("FleetIds")))
		{
			for (const TSharedPtr<FJsonValue>& FleetId : JsonObject->GetArrayField(TEXT("FleetIds")))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FleetId->AsString());
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Failed to get FleetId"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to deserialize JSON object"));
	}
}
