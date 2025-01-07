// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "Game/Log/Log.h"
#include "GameFramework/GameMode.h"
#include "UhuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DRONECOON_API AUhuGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AUhuGameMode();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	// Process Parameters needs to remain in scope for the lifetime of the app
	FProcessParameters ProcessParameters;

	void InitGameLift();

	void SetServerParameters(FServerParameters& OutServerParameters);
	void ParseCommandLinePort(int32& OutPort);
	
};
