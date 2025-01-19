// Copyright by MykeUhu


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UPortalManager::JoinGameSession()
{
	BroadcastJoinGameSessionMessage.Broadcast(TEXT("Searching for Game Session ..."), false);

	check (APIData);
	// Erstellt eine HTTP-Anfrage mithilfe des HttpModules (Singleton zur Handhabung von HTTP-Anfragen in Unreal Engine).
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	// Verbindet die Methode zur Verarbeitung der Antwort (ListFleets_Response) mit der Anfrage.
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::FindOrCreateGameSession_Response);

	// Ruft die URL für den "FindOrCreateGameSession"-Endpunkt basierend auf Gameplay-Tags aus den API-Daten ab.
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);

	// Setzt die URL, Methode und Header für die HTTP-Anfrage.
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST")); // HTTP-Methode GET zum Abrufen von Daten.
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // Content-Type wird auf JSON gesetzt.
    
	// Startet die Verarbeitung der Anfrage.
	Request->ProcessRequest();
}

void UPortalManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
	// JsonObject wird verwendet, um die deserialisierten JSON-Daten zu speichern.
	TSharedPtr<FJsonObject> JsonObject;

	// Erstellt einen JSON-Reader, der den JSON-Inhalt der Antwort liest.
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// Versucht, die JSON-Daten in JsonObject zu deserialisieren.
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
		FDS_GameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession);

		const FString GameSessionStatus = GameSession.Status;
		const FString GameSessionId = GameSession.GameSessionId;
		
		HandleGameSessionStatus(GameSessionStatus, GameSessionId);

		GameSession.Dump();
	}
}



void UPortalManager::HandleGameSessionStatus( const FString& Status, const FString& SessionId)
{
	
	if (Status.Equals(TEXT("ACTIVE")))
	{
		BroadcastJoinGameSessionMessage.Broadcast(TEXT("Found active Game Session. Creating a PlayerSession..."), false);
		TryCreatePlayerSession(GetUniquePlayerID(), SessionId);
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		FTimerDelegate CreateSessionDelegate;
		
		CreateSessionDelegate.BindLambda(
			[this]
			()
			{
				JoinGameSession();
			});
		
		// Umweg über PlayerController da wir hier keine etWorldTimerManager() aufrufen können wegen UObject oder so
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(CreateSessionTimer, CreateSessionDelegate, 0.5, false);
		}
	}
	else
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
}

void UPortalManager::TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId)
{
	check (APIData);
	// Erstellt eine HTTP-Anfrage mithilfe des HttpModules (Singleton zur Handhabung von HTTP-Anfragen in Unreal Engine).
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	// Verbindet die Methode zur Verarbeitung der Antwort (ListFleets_Response) mit der Anfrage.
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::CreatePlayerSession_Response);

	// Ruft die URL für den "FindOrCreateGameSession"-Endpunkt basierend auf Gameplay-Tags aus den API-Daten ab.
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::CreatePlayerSession);

	// Setzt die URL, Methode und Header für die HTTP-Anfrage.
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("POST")); // HTTP-Methode GET zum Abrufen von Daten.
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // Content-Type wird auf JSON gesetzt.

	// Map wird benötig um die Paare zu erzeugen, Hilfsfunktion in UHTTPRequestManager::SerializeJsonContent
	TMap<FString, FString> Params = {
		{TEXT("playerId"), PlayerId},
		{TEXT("gameSessionId"), GameSessionId}
	};
	const FString Content = SerializeJsonContent(Params);
	
	Request->SetContentAsString(Content);
	
	// Startet die Verarbeitung der Anfrage.
	Request->ProcessRequest();
}

void UPortalManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}
	// JsonObject wird verwendet, um die deserialisierten JSON-Daten zu speichern.
	TSharedPtr<FJsonObject> JsonObject;

	// Erstellt einen JSON-Reader, der den JSON-Inhalt der Antwort liest.
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// Versucht, die JSON-Daten in JsonObject zu deserialisieren.
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			BroadcastJoinGameSessionMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
		FDS_PlayerSession PlayerSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSession);

		PlayerSession.Dump();
		
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
			
		{
			FInputModeGameOnly InputModeData;
			LocalPlayerController->SetInputMode(InputModeData);
			LocalPlayerController->SetShowMouseCursor(false);
		}

		const FString IpAndPort = PlayerSession.IpAddress + TEXT(":") + FString::FromInt(PlayerSession.Port);
		const FName Address(*IpAndPort);
		UGameplayStatics::OpenLevel(this, Address);
	}

}


// Platzhalter Funktion, bis wir Login, Registrieren erstellt haben
FString UPortalManager::GetUniquePlayerID() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		APlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(LocalPlayerState) && LocalPlayerState->GetUniqueId().IsValid())
		{
			return TEXT("Player_") + FString::FromInt(LocalPlayerState->GetUniqueID());
		}
	}
	return FString();
}
