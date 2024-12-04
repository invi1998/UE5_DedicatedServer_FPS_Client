// // Copyright invi1998

#include "HTTPRequestTypes.h"

#include "DedicatedServers/DedicatedServers.h"

void FDSMeataData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("metadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("HTTP Status Code: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("Request ID: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("Total Retry Delay: %f"), totalRetryDelay);
}

void FDSListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ListFleetsResponse:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetIds:"));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("  %s"), *FleetId);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);
}
