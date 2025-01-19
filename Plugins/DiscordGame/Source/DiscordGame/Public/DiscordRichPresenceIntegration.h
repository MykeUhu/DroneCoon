#pragma once

#include "CoreMinimal.h"
#include "DiscordGameSubsystem.h"
#include "DiscordPresenceDataAsset.h"
//#include "PackageMigrationContext.h"
#include "DiscordRichPresenceIntegration.generated.h"

/**
 * Enum representing the privacy mode for a party in Discord.
 * Public: Party is visible to all users.
 * Private: Party is restricted to invited users only.
 */
UENUM(BlueprintType)
enum class EPartyPrivacyMode  : uint8
{
	Public   UMETA(DisplayName = "Public"),
	Private  UMETA(DisplayName = "Private")
};

/**
 * Structure holding party-related information for Discord's Rich Presence.
 * This structure encapsulates the maximum and current party sizes, state, secret, and ID.
 */
USTRUCT(BlueprintType)
struct FDiscordPartyInfoResult
{
	GENERATED_BODY()

	/** Maximum number of players allowed in the party. */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	FString MaxPartySize = "";

	/** Current number of players in the party. */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	FString CurrentPartySize = "";

	/** The current state of the party (e.g., "Playing a game"). */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	FString State = "";

	/** The secret associated with the party, used for invitations. */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	FString PartySecret = "";

	/** Unique ID of the party. */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	FString PartyId = "";

	/** The privacy mode of the party, either Public or Private. */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	EPartyPrivacyMode PartyPrivacy = EPartyPrivacyMode::Public;

	/** Indicates whether setting the party information was successful. */
	UPROPERTY(BlueprintReadOnly, Category = "Discord|Party")
	bool bSuccess = false; 
};

/**
 * Custom class for integrating Discord's Rich Presence functionality.
 * This class allows dynamic updates of Discord Rich Presence based on game activities and player state.
 */
UCLASS()
class DISCORDGAME_API UDiscordRichPresenceIntegration : public UDiscordGameSubsystem
{
	GENERATED_BODY()

public:
	/** Retrieves the Rich Presence subsystem instance, or nullptr if Discord is unavailable. */
	static UDiscordRichPresenceIntegration* Get();

	/** Retrieves the Rich Presence subsystem instance, ensuring it is valid. Throws if invalid. */
	static UDiscordRichPresenceIntegration* GetChecked();

	/** Default constructor to initialize subsystem settings. */
	UDiscordRichPresenceIntegration();

	//~ Begin UDiscordGameSubsystem Interface
	/** Called when the Discord Core is successfully created. Used to initialize the Rich Presence functionality. */
	virtual void NativeOnDiscordCoreCreated() override;

	/** Called when the Discord Core is reset or disconnected. Used to clean up or reset Rich Presence data. */
	virtual void NativeOnDiscordCoreReset() override;
	//~ End UDiscordGameSubsystem Interface

	/** Converts a party privacy mode enum to a string representation. */
	FString EnumToString(EPartyPrivacyMode Privacy);

	/** Updates Discord's Rich Presence with the provided activity information. */
	UFUNCTION(BlueprintCallable, Category = "Discord|Rich Presence")
	bool UpdateActivity(const FString& Details,
						const FString& LargeImageAsset,
						const FString& LargeImageText,
						const FString& SmallImageAsset,
						const FString& SmallImageText,
						const FString& MaxPartySizeText = TEXT(""),
						const FString& PartySizeText = TEXT(""),
						const FString& PartyState = TEXT(""),
						const FString& PartySecret = TEXT(""),
						const FString& PartyId = TEXT(""),
						EPartyPrivacyMode PartyPrivacy = EPartyPrivacyMode::Public);

	/** Sets the party information and updates the Rich Presence activity accordingly. */
	UFUNCTION(BlueprintPure, Category="Discord|Party")
	FDiscordPartyInfoResult SetPartyInformation(
		const FString& MaxPartySize,
		const FString& CurrentPartySize,
		const FString& State,
		const FString& PartySecret,
		const FString& PartyId,
		EPartyPrivacyMode PartyPrivacy
	);

	/** Updates Discord's Rich Presence based on data from a specified presence data asset. */
	UFUNCTION(BlueprintCallable, Category = "Discord|Rich Presence")
	void UpdateActivityFromAsset(UDiscordPresenceDataAsset* PresenceData,
		const FString& Key,
		const FDiscordPartyInfoResult& PartyInfo);

private:
	/* Is Timer set? for first Lauch */
	bool bIsTimerSet = false;;
	
	/** Reference to the data asset containing Rich Presence configuration details. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord|Rich Presence", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDiscordPresenceDataAsset> PresenceDataAsset;
};
