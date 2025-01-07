#pragma once

#include "CoreMinimal.h"

/**
 * Deklariere eine Log-Kategorie für das gesamte Projekt.
 */
DECLARE_LOG_CATEGORY_EXTERN(UhuLog, Log, All);

/**
 * Ein Makro für konsistentes Logging, das die Funktion und das Log-Level anzeigt.
 * Nutze UHU_LOG(Log, ...) für normale Logs, UHU_LOG(Warning, ...) für Warnungen, 
 * und UHU_LOG(Error, ...) für Fehler.
 */
#define UHU_LOG(Verbosity, Format, ...) \
UE_LOG(UhuLog, Verbosity, TEXT("%s: ") Format, *FString(__FUNCTION__), ##__VA_ARGS__)
