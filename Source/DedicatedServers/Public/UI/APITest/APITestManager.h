// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "APITestManager.generated.h"

/**
 * 
 */
UCLASS(BLueprintable)
class DEDICATEDSERVERS_API UAPITestManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ListFleetsButtonClicked();
};
