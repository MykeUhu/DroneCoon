// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "UhuCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;


UCLASS()
class DRONECOON_API AUhuCharacter : public ACharacter
{
	GENERATED_BODY()
    
public:
	AUhuCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_PlayerState() override;
	virtual void BeginDestroy() override;
	virtual void UnPossessed() override;
    
protected:
	virtual void BeginPlay() override;

private:
	bool IsLocalThirdPerson() const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh3P;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;
        
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
};

