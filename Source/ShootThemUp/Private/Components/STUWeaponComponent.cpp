// Gorky Deathmatch Game, All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "STUEquipFinishedAnimNotify.h"
#include "STUReloadAnimNotify.h"
#include "GameFramework/Character.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr int32 WEAPON_NUM = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentWeaponIndex = 0;
}

TSubclassOf<ASTUBaseWeapon> USTUWeaponComponent::GetCurrentWeaponType() const
{
	return CurrentWeapon ? CurrentWeapon->GetClass() : nullptr;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WEAPON_NUM, TEXT("Our character can hold only %i weapons"), WEAPON_NUM);

	SpawnWeapons();
	InitAnimations();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		if (!Weapon) continue;
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}

	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(GetOwner());
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;

	if (CurrentWeapon == AmmoEmptyWeapon)
		ChangeClip();
	else
	{
		for (const auto Weapon : Weapons)
			if (Weapon == AmmoEmptyWeapon) Weapon->ChangeClip();
	}
}

void USTUWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadWeaponAnimMontage);
}

void USTUWeaponComponent::Reload()
{
	ChangeClip();
}

float USTUWeaponComponent::GetCurrentSpread()
{
	return CurrentWeapon ? CurrentWeapon->GetCurrentSpread() : 0.0f;
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& Data) const
{
	if (CurrentWeapon)
	{
		Data = CurrentWeapon->GetWeaponUIData();
		return true;
	}

	return false;
}

bool USTUWeaponComponent::GetWeaponAmmoData(FAmmoData& Data) const
{
	if (CurrentWeapon)
	{
		Data = CurrentWeapon->GetAmmoData();
		return true;
	}

	return false;
}
bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimationInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && CanEquip() && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && CanEquip() && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipWeaponAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip animation is null! Please check your animation montage"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadAnimNotify>(OneWeaponData.ReloadWeaponAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload animation is null! Please check your animation montage"));
			checkNoEntry();
		}

		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	EquipAnimationInProgress = false;
}
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	ReloadAnimInProgress = false;
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) return;

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
	CurrentReloadWeaponAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadWeaponAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponAttachSocketName);
	EquipAnimationInProgress = true;
	PlayAnimMontage(EquipWeaponAnimMontage);
}

void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}