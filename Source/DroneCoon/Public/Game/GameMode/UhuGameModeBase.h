#pragma once

#include "CoreMinimal.h"
#include "Game/GameMode/DS_GameMode.h"
#include "UhuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DRONECOON_API AUhuGameModeBase : public ADS_GameMode
{
	GENERATED_BODY()
	
public:
	AUhuGameModeBase();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:


};
