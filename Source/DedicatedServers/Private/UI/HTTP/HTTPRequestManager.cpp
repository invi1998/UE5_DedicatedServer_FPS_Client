// // Copyright invi1998


#include "UI/HTTP/HTTPRequestManager.h"

#include "HTTPRequestTypes.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"

bool UHTTPRequestManager::ContainErrors(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown error type");
		FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown error message");
		UE_LOG(LogDedicatedServers, Error, TEXT("ErrorType: %s, ErrorMessage: %s"), *ErrorType, *ErrorMessage);
		return true;
	}
	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown fault");
		UE_LOG(LogDedicatedServers, Error, TEXT("ErrorType: %s"), *ErrorType);
		return true;
	}
	return false;
}

void UHTTPRequestManager::DumpMetaData(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		TSharedPtr<FJsonObject> MetaDataObject = JsonObject->GetObjectField(TEXT("$metadata"));
		// 将元数据转换为FDSMeataData结构
		FDSMeataData DSMetaData;
		FJsonObjectConverter::JsonObjectToUStruct<FDSMeataData>(MetaDataObject.ToSharedRef(), &DSMetaData, 0, 0);

		DSMetaData.Dump();
	}
}

FString UHTTPRequestManager::SerializeJsonContent(const TMap<FString, FString>& Params)
{
	// Request->SetContentAsString(FString::Printf(TEXT("{\"gameSessionId\":\"%s\",\"playerId\":\"%s\"}"), *GameSessionId, *PlayerId));
	// 或者通过创建一个TSharedPtr<FJsonObject>对象，然后序列化为字符串
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	for (const TPair<FString, FString>& Param : Params)
	{
		JsonObject->SetStringField(Param.Key, Param.Value);
	}
	
	FString ContentString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ContentString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	return ContentString;
}

