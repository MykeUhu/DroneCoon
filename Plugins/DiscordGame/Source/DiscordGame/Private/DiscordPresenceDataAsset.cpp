// Copyright (c) 2024 MykeUhu

#include "DiscordGame/Public/DiscordPresenceDataAsset.h"

bool UDiscordPresenceDataAsset::GetPresenceByKey(const FString& Key, FDiscordPresenceInfo& OutPresenceInfo) const
{
	// Iterate through all presence entries in the data asset
	for (const FDiscordPresenceInfo& Info : PresenceEntries)
	{
		// Check if the current entry's key matches the requested key
		if (Info.Key.ToString() == Key)
		{
			// If a match is found, set the output parameter and return true
			OutPresenceInfo = Info;
			return true;
		}
	}

	// If no matching key is found, log a warning and return false
	UE_LOG(LogTemp, Warning, TEXT("Presence key not found: %s"), *Key);
	return false;
}