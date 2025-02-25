﻿// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTPRequestManager.generated.h"

class UAPIData;
class FJsonObject;

/**
 * 
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> APIData;

	bool ContainsErrors(TSharedPtr<FJsonObject>	JsonObject);

	void DumpMetaData(TSharedPtr<FJsonObject>JsonObject);

	FString SerializeJsonContent(const TMap<FString, FString>& Params);
};
