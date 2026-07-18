// Gorky Deathmatch Game, All Rights Reserved.

#include "Pickups/STUHealthPickup.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "GameFramework/Pawn.h"

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = FStuUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->TryToAddHealth(HealthAmount);
}