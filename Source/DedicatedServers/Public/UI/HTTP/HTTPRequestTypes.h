﻿#pragma once

#include "HTTPRequestTypes.generated.h"

namespace HTTPStatusMessages
{
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong;
}

USTRUCT()
struct FDS_MetaData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 httpStatusCode{};

	UPROPERTY()
	FString requestId{};

	UPROPERTY()
	int32 attempts{};

	UPROPERTY()
	double totalRetryDelay{};

	void Dump() const;
};

USTRUCT()
struct FDS_ListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds{};

	UPROPERTY()
	FString NextToken{};

	void Dump() const;
};

USTRUCT()
struct FDS_GameSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};

	UPROPERTY()
	FString CreatorId{};

	UPROPERTY()
	int32 CurrentPlayerSessionCount{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	TMap<FString, FString> GameProperties{};

	UPROPERTY()
	FString GameSessionData{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString Location{};

	UPROPERTY()
	FString MatchmakerData{};

	UPROPERTY()
	int32 MaximumPlayerSessionCount{};

	UPROPERTY()
	FString Name{};

	UPROPERTY()
	FString PlayerSessionCreationPolicy{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString StatusReason{};

	UPROPERTY()
	FString TerminationTime{};

	void Dump() const;
};

USTRUCT()
struct FDS_PlayerSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString PlayerData{};

	UPROPERTY()
	FString PlayerId{};

	UPROPERTY()
	FString PlayerSessionId{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString TerminationTime{};

	// Method to log the property values
	void Dump() const;
	
};