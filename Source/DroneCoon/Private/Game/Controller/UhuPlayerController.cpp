// Copyright by MykeUhu

#include "Game/Controller/UhuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AUhuPlayerController::AUhuPlayerController()
{
	// Dieser PlayerController wird über das Netzwerk repliziert
	bReplicates = true;
	bPawnIsAlive = true;
}

void AUhuPlayerController::OnPossess(APawn* InPawn)
{
	// Wird aufgerufen, wenn der Controller das Pawn übernimmt.
	// Der Standardcode wird hier mit 'Super::OnPossess' aufgerufen.
	Super::OnPossess(InPawn);
	bPawnIsAlive = true;
}

void AUhuPlayerController::OnRep_PlayerState()
{
	// Wird aufgerufen, wenn der PlayerState des Spielers repliziert wird.
	// Diese Funktion ermöglicht die Reaktion auf Änderungen im PlayerState, falls benötigt.
	Super::OnRep_PlayerState();
	// Hier könnte man Events auslösen, wenn der PlayerState repliziert wird, wie z.B. UI-Aktualisierungen.
	OnPlayerStateReplicated.Broadcast();
}

void AUhuPlayerController::BeginPlay()
{
	// BeginPlay wird aufgerufen, wenn der Controller zum ersten Mal im Spiel ist.
	Super::BeginPlay();

	// Holt das Enhanced Input Subsystem des lokalen Spielers.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		// Fügt das Input Mapping Context zu diesem Subsystem hinzu, damit Eingaben mit diesem Context verarbeitet werden können.
		Subsystem->AddMappingContext(UhuIMC, 0);
	}
}

void AUhuPlayerController::SetupInputComponent()
{
	// SetupInputComponent wird aufgerufen, um alle Eingabebindungen zu konfigurieren.
	Super::SetupInputComponent();

	// Holt das Enhanced Input Component, das mit dem InputComponent des Controllers verbunden ist.
	UEnhancedInputComponent* UhuInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Bindet die Move-Action auf das Input Move Event. Dies wird jedes Mal ausgelöst, wenn die Eingabe für die Bewegung erfolgt.
	UhuInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Input_Move);

	// Bindet die Look-Action auf das Input Look Event. Dies wird jedes Mal ausgelöst, wenn die Eingabe für die Blickrichtung erfolgt.
	UhuInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Input_Look);
}

void AUhuPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!bPawnIsAlive) return;
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AUhuPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}
