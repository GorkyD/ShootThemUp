// Gorky Deathmatch Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFxComponent.generated.h"

class UPhysicalMaterial;
class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFxComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	USTUWeaponFxComponent();

	void PlayImpactFx(const FHitResult& HitResult);

  protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* DefaultEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TMap<UPhysicalMaterial*, UNiagaraSystem*> EffectsMap;
};