// Copyright by MykeUhu


#include "UhuCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AUhuCharacter::AUhuCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Disable controller rotation affecting character directly
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Enable replication for multiplayer scenarios
	bReplicates = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
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

	// Geschwindigkeit und Luftstatus überprüfen
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed > 0.f || bIsInAir) // Charakter bewegt sich oder springt
	{
		bUseControllerRotationYaw = true;

		// Berechne die Bewegungsrichtung für Animationen
		FRotator AimRotation = GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(GetVelocity());
	}
	else // Charakter steht still
	{
		bUseControllerRotationYaw = false;
	}
}

void AUhuCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AUhuCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	// Kein spezielles Aufräumen nötig, erst wenn durch Kampf und "Tod" Inventar aufgeräumt werden soll
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

