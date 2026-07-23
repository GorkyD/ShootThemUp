// Gorky Deathmatch Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Default UMETA(DisplayName = "Default"),
	Riffle UMETA(DisplayName = "Riffle"),
	Launcher UMETA(DisplayName = "Launcher")
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

  public:
	ASTUBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual float GetCurrentSpread();

	virtual void StartFire();
	virtual void StopFire();

	FWeaponUIData GetWeaponUIData() const;
	FAmmoData GetAmmoData() const;

	EWeaponType GetWeaponType() const;

	void ChangeClip();

	bool TryToAddAmmo(int32 ClipsAmount);
	bool CanReload() const;

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData WeaponUIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmoData{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Default;

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	virtual void MakeShot();

	APlayerController* GetPlayerController() const;
	FVector GetMuzzleWorldLocation() const;

	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
	void DecreaseAmmo();

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;

  private:
	FAmmoData CurrentAmmoData;
};
