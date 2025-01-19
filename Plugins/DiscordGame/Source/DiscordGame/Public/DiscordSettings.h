// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DiscordSettings.generated.h"

/**
 * Discord Settings
 *
 * Provides configuration settings for the Discord Game Integration plugin.
 * This class allows you to define and manage settings, such as the Discord Application ID,
 * directly from the Unreal Engine editor under the "Plugins" category.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Discord Game"))
class DISCORDGAME_API UDiscordSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Default constructor to initialize default values. */
	UDiscordSettings();

	/** 
	 * The Discord Application ID associated with your game. 
	 * This ID is required to enable Discord Rich Presence and related features.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Discord", meta = (DisplayName = "Discord Application ID"))
	int64 ApplicationId;

	/**
	 * Retrieves the category name under which these settings appear in the editor.
	 * 
	 * @return The category name as an FName.
	 */
	virtual FName GetCategoryName() const override { return FName(TEXT("Plugins")); }
};
