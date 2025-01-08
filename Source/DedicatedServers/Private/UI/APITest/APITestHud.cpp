// Copyright by MykeUhu


#include "UI/APITest/APITestHud.h"

#include "UI/APITest/APITestOverlay.h"

void AAPITestHud::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) && APITestOverlayClass)
	{
		APITestOverlay = CreateWidget<UAPITestOverlay>(PlayerController, APITestOverlayClass);
		APITestOverlay->AddToViewport();
	}
}
