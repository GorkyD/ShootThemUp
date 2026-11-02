// Gorky Deathmatch Game, All Rights Reserved.

#include "Weapon/Components/STUWeaponFxComponent.h"
#include "NiagaraFunctionLibrary.h"

USTUWeaponFxComponent::USTUWeaponFxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTUWeaponFxComponent::PlayImpactFx(const FHitResult& HitResult)
{
	auto Effect = DefaultEffect;
	if (HitResult.PhysMaterial.IsValid())
	{
		const auto PhysMaterial = HitResult.PhysMaterial.Get();
		if (EffectsMap.Contains(PhysMaterial))
		{
			Effect = EffectsMap[PhysMaterial];
		}
	};

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}
