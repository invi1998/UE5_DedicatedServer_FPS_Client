// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSLocalPlayerSubsystem.h"

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
	OnSignInStatusMessageDelegate.Broadcast(TEXT("账号登录中（Signing in）..."), false);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::PortalAPI::SignIn);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
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
	OnSignUpStatusMessageDelegate.Broadcast(TEXT("账号注册中（Signing up）..."), false);
	checkf(APIData, TEXT("APIData is nullptr"));

	LastSignUpUsername = Username;
	LastSignUpPassword = Password;
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
	OnConfirmAccountStatusMessageDelegate.Broadcast(TEXT("正在确认账户中（Confirming account）..."), false);
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

void UPortalManager::RefreshToken(const FString& RefreshToken)
{
	checkf(APIData, TEXT("APIData is nullptr"));
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_url = APIData->GetAPIEndPoint(DedicatedServersTags::PortalAPI::SignIn);
	Request->SetURL(API_url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 填充API请求参数
	const TMap<FString, FString> Params = {
		{TEXT("refreshToken"), RefreshToken}
	};

	const FString ContentString = SerializeJsonContent(Params);
	Request->SetContentAsString(ContentString);

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::ResendCode_Response);

	Request->ProcessRequest();
}

void UPortalManager::SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnSignInStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		return;
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			OnSignInStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return;
		}

		FDSInitiateAuthResponse LastSignInResponse{};
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LastSignInResponse);

		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
		if (IsValid(LocalPlayerSubsystem))
		{
			LocalPlayerSubsystem->InitializeToken(LastSignInResponse.AuthenticationResult, this);
		}
		
		OnSignInCompleteDelegate.Broadcast();
	}
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
			if (JsonObject->HasField(TEXT("errorType")) && JsonObject->GetStringField(TEXT("errorType")).Equals(TEXT("UsernameExistsException")))
			{
				const FString ErrorMessage = TEXT("用户名已存在，请更换用户名（Username already exists, please change username）");
				OnSignUpStatusMessageDelegate.Broadcast(ErrorMessage, true);
				return;
			}
			if (JsonObject->HasField(TEXT("errorType")) && JsonObject->GetStringField(TEXT("errorType")).Equals(TEXT("UserLambdaValidationException")))
			{
				const FString ErrorMessage = TEXT("该邮箱已被注册，请更换邮箱（Email already exists, please change email）");
				OnSignUpStatusMessageDelegate.Broadcast(ErrorMessage, true);
				return;
			}
			OnSignUpStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return;
		}

		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LastSignUpResponse);
		
		// 切换到验证码页面
		// 等待2S，然后切换到验证码页面
		OnSignUpCompleteDelegate.Broadcast();
	}
}

void UPortalManager::ConfirmAccount_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnConfirmAccountStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		return;
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject))
		{
			if (JsonObject->HasField(TEXT("name")) && JsonObject->GetStringField(TEXT("name")).Equals(TEXT("CodeMismatchException")))
			{
				const FString ErrorMessage = TEXT("验证码错误，请重新输入（Code mismatch, please try again）");
				OnConfirmAccountStatusMessageDelegate.Broadcast(ErrorMessage, true);
				return;
			}
			else
			{
				OnConfirmAccountStatusMessageDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
				return;
			}
		}

		OnConfirmAccountStatusMessageDelegate.Broadcast("账户确认成功（Account confirmed）...", false);
		OnConfirmAccountCompleteDelegate.Broadcast();
	}
}

void UPortalManager::ResendCode_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainErrors(JsonObject)) return;

		FDSInitiateAuthResponse LastSignInResponse{};
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LastSignInResponse);

		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
		if (IsValid(LocalPlayerSubsystem))
		{
			LocalPlayerSubsystem->UpdateToken(LastSignInResponse.AuthenticationResult.AccessToken, LastSignInResponse.AuthenticationResult.IdToken);
		}
	}
}


