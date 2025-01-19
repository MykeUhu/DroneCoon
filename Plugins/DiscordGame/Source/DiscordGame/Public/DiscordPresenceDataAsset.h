#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DiscordPresenceDataAsset.generated.h"

/**
 * Struct containing information for Discord Rich Presence.
 */
USTRUCT(BlueprintType)
struct FDiscordPresenceInfo
{
	GENERATED_BODY()

	/** Primary key identifying this presence entry. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	FName Key;

	/** What the Player is doing currently. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	FString Details;

	/** Key for the large image asset in Discord Developer settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	FString LargeImageAsset;

	/** Tooltip text displayed for the large image. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	FString LargeImageText;

	/** Key for the small image asset in Discord Developer settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	FString SmallImageAsset;

	/** Tooltip text displayed for the small image. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	FString SmallImageText;
};



/**
 * Data Asset containing Discord Rich Presence entries.
 * 
 * This asset allows defining multiple Rich Presence configurations
 * for use with Discord integration in your game.
 */
UCLASS()
class DISCORDGAME_API UDiscordPresenceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Array of all Rich Presence entries defined in this asset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Discord Presence")
	TArray<FDiscordPresenceInfo> PresenceEntries;

	/**
	 * Searches for Rich Presence information by the given key.
	 *
	 * @param Key The key to search for.
	 * @param OutPresenceInfo Outputs the found Rich Presence information, if available.
	 * @return True if an entry with the specified key is found; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Discord Presence")
	bool GetPresenceByKey(const FString& Key, FDiscordPresenceInfo& OutPresenceInfo) const;
};
