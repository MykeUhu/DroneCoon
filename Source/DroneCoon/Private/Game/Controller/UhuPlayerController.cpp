// Copyright by MykeUhu

#include "Game/Controller/UhuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/UhuCharacter.h"

AUhuPlayerController::AUhuPlayerController()
{
    bReplicates = true;
    bPawnIsAlive = true;
    bIsMouseButtonPressed = false;
}

void AUhuPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    bPawnIsAlive = true;
}

void AUhuPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    OnPlayerStateReplicated.Broadcast();
}

void AUhuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(UhuIMC, 0);
    }
}

void AUhuPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* UhuInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    UhuInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Input_Move);
    UhuInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Input_LookAndRotate);
}

void AUhuPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // Removed existing camera rotation logic, as it's now handled in Input_LookAndRotate
}

void AUhuPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
    if (!bPawnIsAlive) return;
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AUhuPlayerController::Input_LookAndRotate(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    
    if (bIsMouseButtonPressed)
    {
        // Rotate the character to face the mouse cursor
        AUhuCharacter* ControlledCharacter = Cast<AUhuCharacter>(GetPawn());
        if (ControlledCharacter)
        {
            FVector MouseLocation, MouseDirection;
            if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
            {
                FPlane GroundPlane(ControlledCharacter->GetActorLocation(), FVector::UpVector);
                FVector IntersectionPoint = FMath::RayPlaneIntersection(MouseLocation, MouseDirection, GroundPlane);
                FVector Direction = (IntersectionPoint - ControlledCharacter->GetActorLocation()).GetSafeNormal2D();
                
                if (!Direction.IsNearlyZero())
                {
                    FRotator NewRotation = Direction.Rotation();
                    NewRotation.Pitch = 0.0f;
                    NewRotation.Roll = 0.0f;
                    ControlledCharacter->SetActorRotation(FMath::RInterpTo(ControlledCharacter->GetActorRotation(), NewRotation, GetWorld()->GetDeltaSeconds(), 10.0f));
                }
            }
        }
    }
    else
    {
        // Rotate the camera
        AddYawInput(InputAxisVector.X);
        AddPitchInput(-InputAxisVector.Y);
    }
}


void AUhuPlayerController::Input_MouseButton(const FInputActionValue& InputActionValue)
{
    bIsMouseButtonPressed = InputActionValue.Get<bool>();
    
    // When the mouse button is pressed, set the character to face the mouse cursor immediately
    if (bIsMouseButtonPressed)
    {
        AUhuCharacter* ControlledCharacter = Cast<AUhuCharacter>(GetPawn());
        if (ControlledCharacter)
        {
            FVector MouseLocation, MouseDirection;
            if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
            {
                FPlane GroundPlane(ControlledCharacter->GetActorLocation(), FVector::UpVector);
                FVector IntersectionPoint = FMath::RayPlaneIntersection(MouseLocation, MouseDirection, GroundPlane);
                FVector Direction = (IntersectionPoint - ControlledCharacter->GetActorLocation()).GetSafeNormal2D();
                
                if (!Direction.IsNearlyZero())
                {
                    FRotator NewRotation = Direction.Rotation();
                    NewRotation.Pitch = 0.0f;
                    NewRotation.Roll = 0.0f;
                    ControlledCharacter->SetActorRotation(NewRotation);
                }
            }
        }
    }
}

