// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "UhuPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateReplicated);
/**
 * 
 */
UCLASS()
class DRONECOON_API AUhuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AUhuPlayerController();

	bool bPawnIsAlive;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateReplicated OnPlayerStateReplicated;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> UhuIMC;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
};
