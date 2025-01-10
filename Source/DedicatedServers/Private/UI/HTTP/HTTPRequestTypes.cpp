#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessages
{
	const FString SomethingWentWrong{TEXT("Something went wrong!")};
}

void FDS_MetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT(" "));
	UE_LOG(LogDedicatedServers, Log, TEXT("=================== Begin MetaData ==================="));
	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode             : %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId                  : %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts                   : %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay            : %f"), totalRetryDelay);
	UE_LOG(LogDedicatedServers, Log, TEXT("✔ SUCCESS!"));
	UE_LOG(LogDedicatedServers, Log, TEXT("===================  End MetaData  ==================="));
}

void FDS_ListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT(" "));
	UE_LOG(LogDedicatedServers, Log, TEXT("================ Begin ListFleetResponse =============="));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId                : %s"), *FleetId);
	}
	if (!NextToken.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("NextToken              : %s"), *NextToken);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("✔ SUCCESS!"));
	UE_LOG(LogDedicatedServers, Log, TEXT("=============== End ListFleetResponse ================"));
}


void FDS_GameSession::Dump() const
{
	
	UE_LOG(LogDedicatedServers, Log, TEXT(" "));
	UE_LOG(LogDedicatedServers, Log, TEXT("================== Begin GameSession =================="));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime               : %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("CreatorId                  : %s"), *CreatorId);
	UE_LOG(LogDedicatedServers, Log, TEXT("CurrentPlayerSessionCount  : %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName                    : %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn                   : %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId                    : %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameProperties:"));
	for (const auto& GameProperty : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("GameProperty               : Key=%s, Value=%s"), *GameProperty.Key, *GameProperty.Value);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionData            : %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId              : %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress                  : %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("Location                   : %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("MatchmakerData             : %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("MaximumPlayerSessionCount  : %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("Name                       : %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port                       : %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status                     : %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("StatusReason               : %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime            : %s"), *TerminationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("✔ SUCCESS!"));
	UE_LOG(LogDedicatedServers, Log, TEXT("=================== End GameSession ==================="));
}

void FDS_PlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT(" "));
	UE_LOG(LogDedicatedServers, Log, TEXT("================= Begin PlayerSession ================="));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime               : %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName                    : %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn                   : %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId                    : %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId              : %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress                  : %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerData                 : %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerId                   : %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionId            : %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port                       : %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status                     : %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime            : %s"), *TerminationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("✔ SUCCESS!"));
	UE_LOG(LogDedicatedServers, Log, TEXT("================== End PlayerSession =================="));
}


