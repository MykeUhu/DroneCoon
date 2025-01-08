// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "APITestHud.generated.h"

class UAPITestOverlay;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API AAPITestHud : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPITestOverlay> APITestOverlayClass;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAPITestOverlay> APITestOverlay;
};
