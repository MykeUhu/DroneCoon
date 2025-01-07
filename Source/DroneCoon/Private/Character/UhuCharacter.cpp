// Copyright by MykeUhu

#include "UhuCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AUhuCharacter::AUhuCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    bReplicates = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AUhuCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUhuCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    APlayerController* PlayerController = Cast<APlayerController>(NewController);
    if (IsValid(PlayerController))
    {
        EnableInput(PlayerController);
    }
}

void AUhuCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AUhuCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
}

void AUhuCharacter::BeginDestroy()
{
    Super::BeginDestroy();
}

void AUhuCharacter::UnPossessed()
{
    Super::UnPossessed();
}

void AUhuCharacter::BeginPlay()
{
    Super::BeginPlay();
}

bool AUhuCharacter::IsLocalThirdPerson() const
{
    return IsValid(Controller) && Controller->IsLocalPlayerController();
}

