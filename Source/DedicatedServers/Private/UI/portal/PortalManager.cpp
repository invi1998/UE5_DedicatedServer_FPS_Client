// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UPortalManager::QuitGame()
{
	if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}

void UPortalManager::SignIn(const FString& Username, const FString& Password)
{
	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::PortalAPI::SignIn);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 填充API请求参数
	const TMap<FString, FString> Params = {
		{TEXT("username"), Username},
		{TEXT("password"), Password}
	};

	const FString ContentString = SerializeJsonContent(Params);
	Request->SetContentAsString(ContentString);

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignIn_Response);

	Request->ProcessRequest();
}

void UPortalManager::SignUp(const FString& Username, const FString& Email, const FString& FullName, const FString& Password)
{
	OnSignUpStatusMessageDelegate.Broadcast(TEXT("Signing up..."), false);
	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::PortalAPI::SignUp);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 填充API请求参数
	const TMap<FString, FString> Params = {
		{TEXT("username"), Username},
		{TEXT("email"), Email},
		{TEXT("name"), FullName},
		{TEXT("password"), Password}
	};

	const FString ContentString = SerializeJsonContent(Params);
	Request->SetContentAsString(ContentString);

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignUp_Response);

	Request->ProcessRequest();
}

void UPortalManager::ConfirmAccount(const FString& Code, const FString& Username)
{
	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::PortalAPI::ConfirmAccount);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 填充API请求参数
	const TMap<FString, FString> Params = {
		{TEXT("username"), Username},
		{TEXT("code"), Code}
	};

	const FString ContentString = SerializeJsonContent(Params);
	Request->SetContentAsString(ContentString);

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::ConfirmAccount_Response);

	Request->ProcessRequest();
}

void UPortalManager::ResendCode()
{
}

void UPortalManager::SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void UPortalManager::SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnSignUpStatusMessageDelegate.Broadcast(TEXT("新用户注册失败 （Sign up failed）"), true);
		return;
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			OnSignUpStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return;
		}
		
		FDSSignUpResponse SignUpResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &SignUpResponse);
		SignUpResponse.Dump();

		OnSignUpStatusMessageDelegate.Broadcast("注册成功，正在发送验证码（Sign up successful, sending code）...", false);

		
		
	}
}

void UPortalManager::ConfirmAccount_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}


