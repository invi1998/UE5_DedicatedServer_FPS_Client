// // Copyright invi1998


#include "UI/APITest/APITestManger.h"

#include "HttpModule.h"
#include "Data/API/APIData.h"

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
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ListFleets_Response"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ListFleets_Response Failed"));
	}
}
