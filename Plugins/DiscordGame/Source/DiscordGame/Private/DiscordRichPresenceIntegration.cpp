#include "DiscordRichPresenceIntegration.h"
#include "../DiscordGame.h"

int64 LastTimestamp = 0;

// Static getter for the DiscordRichPresenceIntegration subsystem
UDiscordRichPresenceIntegration* UDiscordRichPresenceIntegration::Get()
{
    return Cast<UDiscordRichPresenceIntegration>(UDiscordGameSubsystem::Get());
}

// Static getter with a check to ensure the subsystem exists
UDiscordRichPresenceIntegration* UDiscordRichPresenceIntegration::GetChecked()
{
    UDiscordRichPresenceIntegration* Subsystem = Get();
    check(Subsystem);
    return Subsystem;
}

// Constructor
UDiscordRichPresenceIntegration::UDiscordRichPresenceIntegration()
{
    MinimumLogLevel = discord::LogLevel::Debug;
    CreateRetryTime = 1.0f;
    /*
    CurrentPartySize = 1;
    MaxPartySize = 1;
    */
}

void UDiscordRichPresenceIntegration::NativeOnDiscordCoreCreated()
{
    Super::NativeOnDiscordCoreCreated();

    // Example: Update activity after Discord core is ready
    // Without Party
    FString Details = "";
    FString LargeImageAsset = "";
    FString LargeImageText = "";
    FString SmallImageAsset = "";
    FString SmallImageText = "";

    // Party relevant settings (declare and initialize variables correctly)
    int32 MaxPartySize = NULL;  // Example value for the maximum party size
    int32 PartySize = NULL;     // Example value for the current party size
    FString PartyState = "";
    FString PartySecret = "";
    FString PartyId = "";

    // Convert party sizes to FString
    FString MaxPartySizeText = FString::Printf(TEXT("%d"), MaxPartySize); 
    FString PartySizeText = FString::Printf(TEXT("%d"), PartySize);

    // Update the activity with the appropriate parameters
    UpdateActivity(
        Details,
        LargeImageAsset,
        LargeImageText,
        SmallImageAsset,
        SmallImageText,
        MaxPartySizeText,
        PartySizeText,     
        PartyState,
        PartySecret,
        PartyId
    );
}


// Called when the Discord core is reset
void UDiscordRichPresenceIntegration::NativeOnDiscordCoreReset()
{
    Super::NativeOnDiscordCoreReset();
}

FDiscordPartyInfoResult UDiscordRichPresenceIntegration::SetPartyInformation(
    const FString& MaxPartySize,
    const FString& CurrentPartySize,
    const FString& State,
    const FString& PartySecret,
    const FString& PartyId,
    EPartyPrivacyMode PartyPrivacy)
{
    // Create a result instance
    FDiscordPartyInfoResult Result;

    // Set party information
    Result.State = State;
    Result.PartySecret = PartySecret;
    Result.PartyId = PartyId;
    Result.MaxPartySize = MaxPartySize;
    Result.CurrentPartySize = CurrentPartySize;
    Result.PartyPrivacy = PartyPrivacy;

    // Optional: Additional logic to validate or calculate data

    // Set success or failure (e.g., set to true if no errors occur)
    Result.bSuccess = true;

    // Log the party information (optional)
    UE_LOG(LogTemp, Log, TEXT("Party Info: State=%s, Id=%s, Privacy=%s, MaxSize=%s, CurrentSize=%s"),
           *Result.State, *Result.PartyId, *EnumToString(Result.PartyPrivacy),
           *Result.MaxPartySize, *Result.CurrentPartySize);

    // Return the result
    return Result;
}

FString UDiscordRichPresenceIntegration::EnumToString(EPartyPrivacyMode Privacy)
{
    switch (Privacy)
    {
    case EPartyPrivacyMode::Public:
        return TEXT("Public");
    case EPartyPrivacyMode::Private:
        return TEXT("Private");
    default:
        return TEXT("Unknown");
    }
}

bool UDiscordRichPresenceIntegration::UpdateActivity(
    const FString& Details,
    const FString& LargeImageAsset,
    const FString& LargeImageText,
    const FString& SmallImageAsset,
    const FString& SmallImageText,
    const FString& MaxPartySize,
    const FString& PartySize,
    const FString& PartyState,
    const FString& PartySecret,
    const FString& PartyId,
    EPartyPrivacyMode PartyPrivacy) // Default value: Public
{
    if (!IsDiscordRunning())
    {
        UE_LOG(LogDiscord, Warning, TEXT("Discord is not running or not initialized properly."));
        return false;
    }

    // Create the Activity object for Discord
    discord::Activity Activity{};
    Activity.SetType(discord::ActivityType::Playing);
    Activity.SetApplicationId(ClientId);

    // Set details
    Activity.SetDetails(TCHAR_TO_UTF8(*Details));

    // Set assets
    discord::ActivityAssets& Assets = Activity.GetAssets();
    Assets.SetLargeImage(TCHAR_TO_UTF8(*LargeImageAsset));
    Assets.SetLargeText(TCHAR_TO_UTF8(*LargeImageText));
    Assets.SetSmallImage(TCHAR_TO_UTF8(*SmallImageAsset));
    Assets.SetSmallText(TCHAR_TO_UTF8(*SmallImageText));
    
    discord::ActivityParty& Party = Activity.GetParty();
    Party.SetId(TCHAR_TO_UTF8(*PartyId));

    // Set privacy
    switch (PartyPrivacy)
    {
    case EPartyPrivacyMode::Public:
        Party.SetPrivacy(discord::ActivityPartyPrivacy::Public);
        break;
    case EPartyPrivacyMode::Private:
        Party.SetPrivacy(discord::ActivityPartyPrivacy::Private);
        break;
    }

    discord::PartySize& PartySizeStruct = Party.GetSize();
    PartySizeStruct.SetCurrentSize(FCString::Atoi(*PartySize));
    PartySizeStruct.SetMaxSize(FCString::Atoi(*MaxPartySize));

    Activity.SetState(TCHAR_TO_UTF8(*PartyState));
    
    // Set timestamps only at first update
    if (!bIsTimerSet)
    {
        discord::ActivityTimestamps& Timestamps = Activity.GetTimestamps();
        LastTimestamp = FDateTime::UtcNow().ToUnixTimestamp();
        Timestamps.SetStart(LastTimestamp);
        bIsTimerSet = true; // Mark timer as set
    }
    else
    {
        discord::ActivityTimestamps& Timestamps = Activity.GetTimestamps();
        Timestamps.SetStart(LastTimestamp); // Ensure timestamp stays the same
    }

    // Update Discord activity
    bool bResult = false;
    DiscordCore().ActivityManager().UpdateActivity(Activity, [this, &bResult](discord::Result Result)
    {
        const FString RequestDescription(TEXT("Updating Activity"));
        LogDiscordResult(Result, RequestDescription);
        bResult = (Result == discord::Result::Ok);
    });

    return bResult;
}

void UDiscordRichPresenceIntegration::UpdateActivityFromAsset(UDiscordPresenceDataAsset* PresenceData,
    const FString& Key,
    const FDiscordPartyInfoResult& PartyInfo)
{
    if (!PresenceData)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateActivityFromAsset: PresenceData is null."));
        return;
    }

    FDiscordPresenceInfo Info;
    if (PresenceData->GetPresenceByKey(Key, Info) && IsDiscordRunning())
    {
        // Check if party information is valid, and if so, set it
        UE_LOG(LogTemp, Log, TEXT("Updating Party Information: MaxSize=%s, CurrentSize=%s, State=%s"),
               *PartyInfo.MaxPartySize, *PartyInfo.CurrentPartySize, *PartyInfo.State);

        // Set party data in Discord
        SetPartyInformation(
            PartyInfo.MaxPartySize,
            PartyInfo.CurrentPartySize,
            PartyInfo.State,
            PartyInfo.PartySecret,
            PartyInfo.PartyId,
            PartyInfo.PartyPrivacy
        );

        // Without Party
        const FString& Details = TCHAR_TO_UTF8(*Info.Details);
        const FString& LargeImageAsset = TCHAR_TO_UTF8(*Info.LargeImageAsset);
        const FString& LargeImageText = TCHAR_TO_UTF8(*Info.LargeImageText);
        const FString& SmallImageAsset = TCHAR_TO_UTF8(*Info.SmallImageAsset);
        const FString& SmallImageText = TCHAR_TO_UTF8(*Info.SmallImageText);
        
        // Party stuff
        const FString& MaxPartySizeText = TCHAR_TO_UTF8(*PartyInfo.MaxPartySize);
        const FString& PartySizeText = TCHAR_TO_UTF8(*PartyInfo.CurrentPartySize);
        const FString& PartyState = TCHAR_TO_UTF8(*PartyInfo.State);
        const FString& PartySecret = TCHAR_TO_UTF8(*PartyInfo.PartySecret);
        const FString& PartyId = TCHAR_TO_UTF8(*PartyInfo.PartyId);

        // TODO: Add party privacy
        UpdateActivity(Details,
        LargeImageAsset,
        LargeImageText,
        SmallImageAsset,
        SmallImageText,
        MaxPartySizeText,
        PartySizeText,     
        PartyState,
        PartySecret,
        PartyId
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateActivityFromAsset: Failed to retrieve presence data or Discord is not running."));
    }
}
