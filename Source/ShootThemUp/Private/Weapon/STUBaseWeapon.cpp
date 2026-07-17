// Gorky Deathmatch Game, All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/HitResult.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMeshComponent);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMeshComponent);
	checkf(DefaultAmmoData.Bullets > 0, TEXT("Bullets count couldn't be less than or equal to zero"));
	checkf(DefaultAmmoData.Clips > 0, TEXT("Clips count couldn't be less than or equal to zero"));
	CurrentAmmoData = DefaultAmmoData;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return CurrentAmmoData.Clips == 0 && !CurrentAmmoData.Infinite && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmoData.Bullets == 0;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmoData.Bullets == 0) return;

	CurrentAmmoData.Bullets--;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast();
	}
}

void ASTUBaseWeapon::ChangeClip()
{
	if (!CurrentAmmoData.Infinite)
	{
		if (CurrentAmmoData.Clips == 0) return;
		CurrentAmmoData.Clips--;
	}

	CurrentAmmoData.Bullets = DefaultAmmoData.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("----------Clip Changed----------"));
}
bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmoData.Bullets < DefaultAmmoData.Bullets && CurrentAmmoData.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo()
{
	FString LogAmmo = "Ammo: " + FString::FromInt(CurrentAmmoData.Bullets) + " / ";
	LogAmmo += CurrentAmmoData.Infinite ? "Infinite" : FString::FromInt(CurrentAmmoData.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *LogAmmo);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::MakeShot()
{
}