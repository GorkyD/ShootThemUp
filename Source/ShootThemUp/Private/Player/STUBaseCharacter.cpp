// Gorky Deathmatch Game, All Rights Reserved.

#include "Player/STUBaseCharacter.h"

#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.h"

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ASTUBaseCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &ASTUBaseCharacter::MoveVertical);

	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
}
bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}
float ASTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
void ASTUBaseCharacter::MoveHorizontal(float amount)
{
	if (amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), amount);
}
void ASTUBaseCharacter::MoveVertical(float amount)
{
	if (amount == 0.0f) return;
	IsMovingForward = amount > 0.0f;
	AddMovementInput(GetActorForwardVector(), amount);
}
void ASTUBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}
