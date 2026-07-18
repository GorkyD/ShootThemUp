// Gorky Deathmatch Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "Animation/AnimMontage.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	USTUWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();

	bool GetWeaponUIData(FWeaponUIData& Data) const;
	bool GetWeaponAmmoData(FAmmoData& Data) const;

  protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipWeaponAnimMontage;

  private:
	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	void PlayAnimMontage(UAnimMontage* Animation) const;
	void EquipWeapon(int32 WeaponIndex);

	void InitAnimations();
	void SpawnWeapons();
	void OnEmptyClip();
	void ChangeClip();

	bool CanEquip() const;
	bool CanFire() const;
	bool CanReload() const;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	UPROPERTY()
	UAnimMontage* CurrentReloadWeaponAnimMontage = nullptr;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	FName WeaponAttachSocketName = "WeaponSocket";
	FName WeaponArmorySocketName = "ArmorySocket";

	int32 CurrentWeaponIndex = 0;

	bool EquipAnimationInProgress = false;
	bool ReloadAnimInProgress = false;
};
