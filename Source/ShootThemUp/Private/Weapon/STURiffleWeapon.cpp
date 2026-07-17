// Gorky Deathmatch Game, All Rights Reserved.

#include "Weapon/STURiffleWeapon.h"
#include "Engine/HitResult.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PlayerController.h"

void ASTURiffleWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURiffleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURiffleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURiffleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5);

		MakeDamage(HitResult);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

bool ASTURiffleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRadius = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRadius);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}
void ASTURiffleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto HitTargetActor = HitResult.GetActor();
	if (!HitTargetActor) return;
	HitTargetActor->TakeDamage(RiffleDamage, FDamageEvent(), GetPlayerController(), this);
}