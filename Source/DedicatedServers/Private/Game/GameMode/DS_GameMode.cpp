// Copyright by MykeUhu


#include "DedicatedServers/Public/Game/GameMode/DS_GameMode.h"

DEFINE_LOG_CATEGORY(LogDS_GameMode);

void ADS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_GAMELIFT
	InitGameLift();
#endif
}

/**
 * @brief Extrahiert Serverparameter aus den Kommandozeilenargumenten.
 * 
 * @param OutServerParameters Referenz zu einer Struktur, die mit den Parametern gefüllt wird.
 */
void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	// Überprüft und extrahiert den Authentifizierungstoken aus den Kommandozeilenparametern
    if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken);
    }

    // Überprüft und extrahiert die Host-ID des GameLift-Servers
    if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId);
    }

    // Überprüft und extrahiert die Fleet-ID von GameLift
    if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId);
    }

    // Überprüft und extrahiert die WebSocket-URL
    if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl);
    }

    // Extrahiert den AWS-Region-Parameter aus den Kommandozeilenparametern
    if (FParse::Value(FCommandLine::Get(), TEXT("-awsregion="), OutServerParameters.m_awsRegion))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("AWS_REGION: %s"), *OutServerParameters.m_awsRegion);
    }

    // Extrahiert den Access Key aus den Kommandozeilenparametern
    if (FParse::Value(FCommandLine::Get(), TEXT("-accesskey="), OutServerParameters.m_accessKey))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("ACCESS_KEY: %s"), *OutServerParameters.m_accessKey);
    }

    // Extrahiert den Secret Key aus den Kommandozeilenparametern
    if (FParse::Value(FCommandLine::Get(), TEXT("-secretkey="), OutServerParameters.m_secretKey))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("SECRET_KEY: %s"), *OutServerParameters.m_secretKey);
    }

    // Extrahiert den SessionToken aus den Kommandozeilenparametern
    if (FParse::Value(FCommandLine::Get(), TEXT("-sessiontoken="), OutServerParameters.m_sessionToken))
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("SESSION_TOKEN: %s"), *OutServerParameters.m_sessionToken);
    }
    
    // The PID of the running process
    OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
    UE_LOG(LogDS_GameMode, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}

/**
 * @brief Extrahiert den Port aus den Kommandozeilenparametern.
 * 
 * @param OutPort Referenz auf die Variable, in der der extrahierte Port gespeichert wird.
 */
void ADS_GameMode::ParseCommandLinePort(int32& OutPort)
{
    TArray<FString> CommandLineTokens;
    TArray<FString> CommandLineSwitches;
    FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
    for (const FString& Switch : CommandLineSwitches)
    {
        FString Key;
        FString Value;
        if (Switch.Split("=", &Key, &Value))
        {
            if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
            {
                OutPort = FCString::Atoi(*Value);
                return;
            }
        }
    }
}

/**
 * @brief Initialisiert das GameLift SDK und konfiguriert serverseitige Parameter und Callbacks.
 */
void ADS_GameMode::InitGameLift()
{
       UE_LOG(LogDS_GameMode, Log, TEXT("Initializing the GameLift Server"));

    // Lädt das GameLift Server SDK-Modul
    FGameLiftServerSDKModule* GameLiftSdkModule =
        &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

    // Definiert die Serverparameter für "GameLift Anywhere fleet". Nicht benötigt für "GameLift managed EC2 fleet".
    FServerParameters ServerParameters;

    SetServerParameters(ServerParameters);

    // InitSDK stellt eine Verbindung zum GameLift-Agent her, um Kommunikation zu ermöglichen.
    // Verwende InitSDK(serverParameters) für eine GameLift Anywhere-Fleet.
    // Verwende InitSDK() für eine GameLift Managed EC2-Fleet.
    GameLiftSdkModule->InitSDK(ServerParameters);

    // Callback: onStartGameSession
    // Wird von GameLift aufgerufen, um eine Spielsession zu aktivieren. 
    // Hier verarbeitet der Server die übergebenen Sitzungsdaten und aktiviert die Sitzung,
    // sobald er bereit ist, Spieler zu empfangen (ActivateGameSession).
    auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
    {
        FString GameSessionId = FString(gameSession.GetGameSessionId());
        UE_LOG(LogDS_GameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
        GameLiftSdkModule->ActivateGameSession();
    };
    ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

    
    // Callback: OnProcessTerminate
    // Wird von GameLift vor dem Herunterfahren des Game-Server-Hosts aufgerufen.
    // Ermöglicht es dem Server, seinen Zustand zu speichern und Abschaltung einzuleiten.
    // Nach Abschluss meldet der Server mit ProcessEnding() die Beendigung an GameLift.
    auto OnProcessTerminate = [=]()
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("Game Server process is terminating."));
        GameLiftSdkModule->ProcessEnding();
    };
    ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

    // Callback: OnHealthCheck
    // Wird alle 60 Sekunden von GameLift aufgerufen, um den Zustand des Servers zu prüfen.
    // Der Server gibt 'true' zurück, wenn er gesund ist, andernfalls 'false'.
    // Muss innerhalb von 60 Sekunden antworten, sonst wird 'false' aufgezeichnet.
    auto OnHealthCheck = []()
    {
        UE_LOG(LogDS_GameMode, Log, TEXT("Performing Health Check"));
        return true;
    };
    ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

    // Der Server bereitet sich darauf vor, dass er bereit ist, Spielsessions zu hosten
    int32 Port = FURL::UrlConfig.DefaultPort;
    ParseCommandLinePort(Port);
    ProcessParameters.port = Port;
        
    // ProcessId setzen: Wird aus den Kommandozeilenparametern entnommen, 
    // falls nicht vorhanden, wird eine einzigartige ProcessId generiert.
    FString glProcessId = "";
    if (FParse::Value(FCommandLine::Get(), TEXT("-processId="), glProcessId))
    {
        ServerParameters.m_processId = TCHAR_TO_UTF8(*glProcessId);
    }
    else
    {
        // Generiere eine einzigartige ProcessId, wenn keine angegeben wurde
        FString UniqueProcessId = FString::Printf(TEXT("ProcessId_%d_%s"), FPlatformTime::Cycles(), *FDateTime::Now().ToString());
        ServerParameters.m_processId = TCHAR_TO_UTF8(*UniqueProcessId);
    }

    // Logge die ProcessId
    UE_LOG(LogDS_GameMode, Log, TEXT("PID: %s"), *ServerParameters.m_processId);

    // Definiere Logdateien
    TArray<FString> logfiles;
    logfiles.Add(TEXT("DroneCoon/Saved/Logs/DroneCoon.log"));
    ProcessParameters.logParameters = logfiles;

    // Signaliere GameLift, dass der Server bereit ist
    UE_LOG(LogDS_GameMode, Log, TEXT("Calling Process Ready"));
    GameLiftSdkModule->ProcessReady(ProcessParameters);
}
