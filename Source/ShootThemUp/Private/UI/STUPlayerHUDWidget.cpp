// Gorky Deathmatch Game, All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"
#include "GameFramework/Pawn.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = GetHealthComponent();
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& Data) const
{
	const auto WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetWeaponUIData(Data);
}

bool USTUPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& Data) const
{
	const auto WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetWeaponAmmoData(Data);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GetHealthComponent();
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return nullptr;

	return FStuUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Player);
}
USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return nullptr;

	return FStuUtils::GetSTUPlayerComponent<USTUHealthComponent>(Player);
}