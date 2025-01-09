// Copyright by MykeUhu

#include "UI/APITest/APITestOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/API/ListFleets/FleetID.h"
#include "UI/API/ListFleets/ListFleetsBox.h"
#include "UI/APITest/APITestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	// Überprüft, ob die Klasse für den API-Test-Manager korrekt definiert wurde.
	check(APITestManagerClass);

	// Erstellt eine Instanz des API-Test-Managers.
	APITestManager = NewObject<UAPITestManager>(this, APITestManagerClass);

	// Überprüft, ob das ListFleetsBox-Objekt und der zugehörige Button korrekt initialisiert wurden.
	check(ListFleetsBox);
	check(ListFleetsBox->Button_ListFleets);

	// Bindet die Klick-Ereignisbehandlung an den Button.
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(this, &UAPITestOverlay::ListFleetsButtonClicked);
}

void UAPITestOverlay::ListFleetsButtonClicked()
{
	// Überprüft, ob der API-Test-Manager korrekt initialisiert wurde.
	check(APITestManager);

	// Bindet die Verarbeitung der Antwort an die Methode OnListFleetsResponseReceived.
	APITestManager->OnListFleetsResponseReceived.AddDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);

	// Startet den Prozess zum Abrufen der Flottenliste.
	APITestManager->ListFleets();

	// Deaktiviert den Button, um mehrfaches Klicken zu verhindern, während auf die Antwort gewartet wird.
	ListFleetsBox->Button_ListFleets->SetIsEnabled(false);
}

void UAPITestOverlay::OnListFleetsResponseReceived(const FDS_ListFleetsResponse& ListFleetsResponse, bool bWasSuccessful)
{
	if (APITestManager->OnListFleetsResponseReceived.IsAlreadyBound(this, &UAPITestOverlay::OnListFleetsResponseReceived))
	{
		APITestManager->OnListFleetsResponseReceived.RemoveDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);
	}
	ListFleetsBox->ScrollBox_ListFleets->ClearChildren();
	
	// Prüft den Status der Antwort und führt die entsprechenden Aktionen aus.
	if (bWasSuccessful)
	{
		for (const FString& FleetID : ListFleetsResponse.FleetIds)
		{
			UFleetID* FleetIDWidget = CreateWidget<UFleetID>(this, FleetIDWidgetClass);
			FleetIDWidget->TextBlock_FleetID->SetText(FText::FromString(FleetID));
			ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIDWidget);
		}
	}
	else
	{
		UFleetID* FleetIDWidget = CreateWidget<UFleetID>(this, FleetIDWidgetClass);
		FleetIDWidget->TextBlock_FleetID->SetText(FText::FromString("Something went wrong!"));
		ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIDWidget);
	}

	// Aktiviert den Button nach Abschluss der Verarbeitung wieder.
	ListFleetsBox->Button_ListFleets->SetIsEnabled(true);
}
