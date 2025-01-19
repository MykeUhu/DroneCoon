// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameMode.h"
#include "DS_GameMode.generated.h"


class UDiscordSubsystem;
DECLARE_LOG_CATEGORY_EXTERN(LogDS_GameMode, Log, All);
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// Process Parameters needs to remain in scope for the lifetime of the app
	FProcessParameters ProcessParameters;

	void InitGameLift();

	void SetServerParameters(FServerParameters& OutServerParameters);
	void ParseCommandLinePort(int32& OutPort);
};