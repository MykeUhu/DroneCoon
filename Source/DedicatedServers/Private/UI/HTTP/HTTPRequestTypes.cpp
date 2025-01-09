#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

void FDS_MetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("=================== Begin MetaData ==================="));
	// %d = Integer, %s = string, %f = double/float
	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
	UE_LOG(LogDedicatedServers, Log, TEXT("===================  End MetaData  ==================="));
}

void FDS_ListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("============== Begin  ListFleetResponse =============="));
	for (const FString& FLeetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FLeetId);
	}
	if (!NextToken.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("=============== End  ListFleetResponse ==============="));
}
