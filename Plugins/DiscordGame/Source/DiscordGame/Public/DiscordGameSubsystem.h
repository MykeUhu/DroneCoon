// Copyright (c) 2024 MykeUhu

#pragma once

#include "CoreMinimal.h"

#include "../discord-cpp/discord.h"
#include "../DiscordGame.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "Containers/Ticker.h"
#include "DiscordGameSubsystem.generated.h"

/**
 * Discord Game Subsystem
 *
 * Manages the Discord GameSDK connection for your game.
 * This subsystem handles loading the Discord GameSDK DLL, maintaining the connection,
 * and managing connection retries. 
 *
 * Extend this subsystem to implement custom game logic using the Discord SDK.
 *
 */
UCLASS(Config=Game, Blueprintable)
class DISCORDGAME_API UDiscordGameSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	/** Retrieve the subsystem instance, or nullptr if the engine is unavailable */
	static UDiscordGameSubsystem* Get() { return GEngine ? GEngine->GetEngineSubsystem<UDiscordGameSubsystem>() : nullptr; }

	// Constructor to set default values
	UDiscordGameSubsystem();

	//~USubsystem interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End of USubsystem interface

	/**
	 * Checks if the Discord GameSDK DLL is successfully loaded.
	 * Note: This does not indicate whether Discord is installed or running.
	 * 
	 * @return true if the DLL is loaded; false otherwise
	 */
	bool IsDiscordSDKLoaded() const { return DiscordGameModule && DiscordGameModule->IsDiscordSDKLoaded(); }

	/**
	 * Checks if Discord is currently running and connected.
	 *
	 * @return true if connected to Discord; false otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool IsDiscordRunning() const { return DiscordCorePtr != nullptr; }

	/**
	 * Provides access to the Discord Core interface.
	 * Ensure that IsDiscordRunning() returns true before calling this method.
	 *
	 * @return Reference to the Discord Core instance
	 */
	FORCEINLINE_DEBUGGABLE discord::Core& DiscordCore() const
	{
		checkf(DiscordCorePtr, TEXT("Discord is not running"));
		return *DiscordCorePtr;
	}
	
	/**
	 * Converts the ClientId to a string for debugging purposes.
	 *
	 * @return FString representation of the ClientId
	 */
	UFUNCTION(BlueprintCallable, Category = "Discord | Debug")
	FString GetClientIdAsString() const;

protected:
	/** Called when a new Discord connection is established. */
	virtual void NativeOnDiscordCoreCreated();

	/** Called when the Discord connection is reset. */
	virtual void NativeOnDiscordCoreReset();

	/** Called when a connection attempt to Discord fails. */
	virtual void NativeOnDiscordConnectError(discord::Result Result);

	/** Handles logging messages from the Discord SDK. */
	virtual void NativeOnDiscordLogMessage(discord::LogLevel Level, const FString& Message) const;

	/** Logs the result of an asynchronous Discord SDK operation. */
	void LogDiscordResult(discord::Result Result, const FString& RequestDescription) const;

	/** Enables or disables ticking for this subsystem. */
	void SetTickEnabled(bool bWantTicking);

	/**
	 * Your game's Discord Application ID (Client ID).
	 */
	uint64 ClientId;

	/**
	 * Minimum log level for Discord SDK messages.
	 */
	discord::LogLevel MinimumLogLevel;

	/**
	 * Minimum time interval (in seconds) between connection retries.
	 */
	UPROPERTY(Config, EditDefaultsOnly)
	float CreateRetryTime;

private:
	/** Handles ticking the subsystem to maintain the Discord connection. */
	bool Tick(float DeltaTime);

	/** Attempts to establish a connection to Discord. */
	void TryCreateDiscordCore(float DeltaTime);

	/** Resets the Discord Core instance, disconnecting if necessary. */
	void ResetDiscordCore();

	/** Pointer to the Discord Game module. */
	FDiscordGameModule* DiscordGameModule {nullptr};

	/** Pointer to the Discord Core instance, if connected. */
	discord::Core* DiscordCorePtr {nullptr};

	/** Handle for the tick delegate. */
	FTSTicker::FDelegateHandle TickDelegateHandle;

	/** Time remaining (in seconds) before the next connection retry. */
	float RetryWaitRemaining {-1.f};

	/** Flag to prevent repetitive error logging for connection issues. */
	bool bLogConnectionErrors {true};
	
};
