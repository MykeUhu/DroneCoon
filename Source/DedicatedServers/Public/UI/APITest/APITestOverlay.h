// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UFleetID;
class UAPITestManager;
class UListFleetsBox;

struct FDS_ListFleetsResponse;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPITestManager> APITestManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFleetID> FleetIDWidgetClass;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListFleetsBox> ListFleetsBox;

	UPROPERTY()
	TObjectPtr<UAPITestManager> APITestManager;

	UFUNCTION()
	void ListFleetsButtonClicked();

	UFUNCTION()
	void OnListFleetsResponseReceived(const FDS_ListFleetsResponse& ListFleetsResponse, bool bWasSuccessful);

};
