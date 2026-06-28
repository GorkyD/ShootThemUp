// Gorky Deathmatch Game, All Rights Reserved.

#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

  public:
	ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

  protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float SpintSpeed = 1000.0f;

	virtual void BeginPlay() override;

  public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

  private:
	void MoveHorizontal(float amount);
	void MoveVertical(float amount);

	void OnStartRunning();
	void OnStopRunning();

	bool IsMovingForward = false;
	bool WantsToRun = false;
};
