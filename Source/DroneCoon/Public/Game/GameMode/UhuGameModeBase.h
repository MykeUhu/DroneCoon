// Copyright by MykeUhu

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
	
	virtual void Tick(float DeltaTime) override;

protected:

private:



};