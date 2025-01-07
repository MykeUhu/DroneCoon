// Copyright by MykeUhu

#include "Game/Controller/UhuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/UhuCharacter.h"

AUhuPlayerController::AUhuPlayerController(): LastLookInput()
{
    bReplicates = true;
    bPawnIsAlive = true;
    bIsMoving = false;
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

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
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

    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
        bIsMoving = true;
    }
    bIsMoving = false;
}

void AUhuPlayerController::Input_LookAndRotate(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    
    if (bIsMoving)
    {
        // Rotate the character to face the mouse cursor
        if (AUhuCharacter* ControlledCharacter = Cast<AUhuCharacter>(GetPawn()))
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