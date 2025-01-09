// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "APIData.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPIData : public UDataAsset
{
	GENERATED_BODY()

public:

	FString GetAPIEndpoint(const FGameplayTag& APIEndpoint);
	
protected:
	// Name für die API - für "Labeling" im Data Asset; wird nicht im Code verwendet.
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;
	
	UPROPERTY(EditDefaultsOnly)
	FString Stage;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> Resources;	
};
