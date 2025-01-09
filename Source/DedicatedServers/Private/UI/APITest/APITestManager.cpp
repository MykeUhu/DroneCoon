// Copyright by MykeUhu

#include "UI/APITest/APITestManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestManager::ListFleets()
{
    // Überprüft, ob APIData korrekt initialisiert ist. Ohne diese Überprüfung könnte das Programm abstürzen.
    check(APIData);

    // Erstellt eine HTTP-Anfrage mithilfe des HttpModules (Singleton zur Handhabung von HTTP-Anfragen in Unreal Engine).
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

    // Verbindet die Methode zur Verarbeitung der Antwort (ListFleets_Response) mit der Anfrage.
    Request->OnProcessRequestComplete().BindUObject(this, &UAPITestManager::ListFleets_Response);
    
    // Ruft die URL für den "List Fleets"-Endpunkt basierend auf Gameplay-Tags aus den API-Daten ab.
    const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::ListFleets);

    // Setzt die URL, Methode und Header für die HTTP-Anfrage.
    Request->SetURL(APIUrl);
    Request->SetVerb(TEXT("GET")); // HTTP-Methode GET zum Abrufen von Daten.
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // Content-Type wird auf JSON gesetzt.
    
    // Startet die Verarbeitung der Anfrage.
    Request->ProcessRequest();
}

void UAPITestManager::ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // Zeigt eine Debug-Nachricht an, dass eine Antwort eingegangen ist.
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "List Fleets Response Received");

    /*
        JSON-Reader Erklärung:
        Ein TSharedRef wird hier verwendet, da ein JSON-Reader für die Verarbeitung erforderlich ist und nicht null sein darf.
        TSharedPtr wäre flexibler, erlaubt aber einen null-Zustand, was in diesem Fall unerwünscht ist.
    */

    // JsonObject wird verwendet, um die deserialisierten JSON-Daten zu speichern.
    TSharedPtr<FJsonObject> JsonObject;

    // Erstellt einen JSON-Reader, der den JSON-Inhalt der Antwort liest.
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    // Versucht, die JSON-Daten in JsonObject zu deserialisieren.
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        // Überprüft, ob die Antwort Fehler enthält, und beendet den Prozess, falls dies der Fall ist.
        if (ContainsErrors(JsonObject))
        {
            OnListFleetsResponseReceived.Broadcast(FDS_ListFleetsResponse(), false);
            return;
        }

        // Gibt zusätzliche Metadaten aus der Antwort für Debugging-Zwecke aus.
        DumpMetaData(JsonObject);

        // Konvertiert das JSON-Objekt in eine C++-Struktur (FDS_ListFleetsResponse).
        FDS_ListFleetsResponse ListFleetsResponse;
        FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &ListFleetsResponse);

        // Debug-Ausgabe der konvertierten Daten.
        ListFleetsResponse.Dump();

        // Signalisiert allen Abonnenten, dass die Antwort verarbeitet wurde.
        OnListFleetsResponseReceived.Broadcast(ListFleetsResponse, true);
    }
}
