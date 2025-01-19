// Copyright (c) 2024 MykeUhu

#include "DiscordGame/Public/DiscordGameSubsystem.h"
#include "../DiscordGame.h"
#include "DiscordGame/Public/DiscordSettings.h"

UDiscordGameSubsystem::UDiscordGameSubsystem()
{
    // Access the Discord settings
    const UDiscordSettings* DiscordSettings = GetDefault<UDiscordSettings>();

    // Retrieve the ApplicationId (known as ClientId in Discord) from the settings
    ClientId = DiscordSettings->ApplicationId;

    // Set default values for log level and retry time
    MinimumLogLevel = discord::LogLevel::Debug;
    CreateRetryTime = 5.0f; // Time in seconds between reconnection attempts
}

bool UDiscordGameSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    TArray<UClass*> ChildClasses;
    GetDerivedClasses(GetClass(), OUT ChildClasses, false);

    UE_LOG(LogDiscord, Verbose, TEXT("Found %i derived classes when attempting to create DiscordGameSubsystem (%s)"), ChildClasses.Num(), *GetClass()->GetName());

    // Only create an instance if there are no override implementations defined
    return ChildClasses.Num() == 0;
}

void UDiscordGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Acquire the DiscordGame module
    DiscordGameModule = FDiscordGameModule::Get();

    // Enable subsystem ticking if the SDK was successfully loaded
    if (IsDiscordSDKLoaded())
    {
        UE_LOG(LogDiscord, Log, TEXT("SDK loaded, enabling subsystem ticking"));
        SetTickEnabled(true);
    }
    else
    {
        UE_LOG(LogDiscord, Error, TEXT("SDK load failed, disabling subsystem"));
    }

    // Check if ClientId is set
    if (ClientId == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ClientId is not set in project settings. Discord integration will not function properly."));
    }
}

void UDiscordGameSubsystem::Deinitialize()
{
    // Stop ticking and reset the DiscordCore
    SetTickEnabled(false);
    ResetDiscordCore();

    DiscordGameModule = nullptr;
    Super::Deinitialize();
}

FString UDiscordGameSubsystem::GetClientIdAsString() const
{
    return FString::Printf(TEXT("%llu"), ClientId);
}

void UDiscordGameSubsystem::NativeOnDiscordCoreCreated()
{
    check(DiscordCorePtr);

    // Set up log hook for Discord SDK
    DiscordCorePtr->SetLogHook(MinimumLogLevel, [this](discord::LogLevel Level, const char* RawMessage)
    {
        const FString Message(UTF8_TO_TCHAR(RawMessage));
        NativeOnDiscordLogMessage(Level, Message);
    });

    // Reset error logging for new connections
    bLogConnectionErrors = true;
}

void UDiscordGameSubsystem::NativeOnDiscordCoreReset()
{
    // Hook for child classes to handle core resets
    // Consider adding implementation or removing if not used
}

void UDiscordGameSubsystem::NativeOnDiscordConnectError(discord::Result Result)
{
    switch (Result)
    {
    case discord::Result::InternalError:
        if (bLogConnectionErrors)
        {
            UE_LOG(LogDiscord, Warning, TEXT("Error(%i) Connecting to Discord; Discord App not running?"), Result);
            bLogConnectionErrors = false;
        }
        else
        {
            UE_LOG(LogDiscord, VeryVerbose, TEXT("Error(%i) Connecting to Discord; Discord App not running?"), Result);
        }
        break;

    default:
        UE_LOG(LogDiscord, Error, TEXT("Error(%i) Connecting to Discord; Unknown error"), Result);
        break;
    }
}

void UDiscordGameSubsystem::NativeOnDiscordLogMessage(discord::LogLevel Level, const FString& Message) const
{
    switch (Level)
    {
    case discord::LogLevel::Debug:
        UE_LOG(LogDiscord, Verbose, TEXT("Discord Debug: %s"), *Message);
        break;
    case discord::LogLevel::Info:
        UE_LOG(LogDiscord, Log, TEXT("Discord Info: %s"), *Message);
        break;
    case discord::LogLevel::Warn:
        UE_LOG(LogDiscord, Warning, TEXT("Discord Warning: %s"), *Message);
        break;
    case discord::LogLevel::Error:
    default:
        UE_LOG(LogDiscord, Error, TEXT("Discord Error: %s"), *Message);
        break;
    }
}

void UDiscordGameSubsystem::LogDiscordResult(discord::Result Result, const FString& RequestDescription) const
{
    if (Result == discord::Result::Ok)
    {
        UE_LOG(LogDiscord, Log, TEXT("Success: %s"), *RequestDescription);
    }
    else
    {
        UE_LOG(LogDiscord, Error, TEXT("Error(%i): %s"), Result, *RequestDescription);
    }
}

void UDiscordGameSubsystem::SetTickEnabled(bool bWantTicking)
{
    if (bWantTicking && !TickDelegateHandle.IsValid())
    {
        TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::Tick));
    }
    else if (!bWantTicking && TickDelegateHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
        TickDelegateHandle.Reset();
    }
}

bool UDiscordGameSubsystem::Tick(float DeltaTime)
{
    if (IsDiscordRunning())
    {
        const discord::Result Result = DiscordCorePtr->RunCallbacks();
        if (Result == discord::Result::NotRunning)
        {
            UE_LOG(LogDiscord, Warning, TEXT("Discord app is no longer running"));
            ResetDiscordCore();
        }
        else if (Result != discord::Result::Ok)
        {
            UE_LOG(LogDiscord, Error, TEXT("Error(%i) Running Callbacks"), Result);
        }
    }
    else if (IsDiscordSDKLoaded())
    {
        TryCreateDiscordCore(DeltaTime);
    }

    return true; // Continue ticking
}

void UDiscordGameSubsystem::TryCreateDiscordCore(float DeltaTime)
{
    RetryWaitRemaining -= DeltaTime;

    if (RetryWaitRemaining <= 0.f)
    {
        const discord::Result Result = discord::Core::Create(ClientId, DiscordCreateFlags_NoRequireDiscord, &DiscordCorePtr);
        if (Result == discord::Result::Ok)
        {
            UE_LOG(LogDiscord, Log, TEXT("Created Discord Core"));
            NativeOnDiscordCoreCreated();
        }
        else
        {
            NativeOnDiscordConnectError(Result);
        }

        RetryWaitRemaining = CreateRetryTime;
    }
}

void UDiscordGameSubsystem::ResetDiscordCore()
{
    if (DiscordCorePtr)
    {
        DiscordCorePtr = nullptr;
        NativeOnDiscordCoreReset();
    }

    RetryWaitRemaining = -1;
}