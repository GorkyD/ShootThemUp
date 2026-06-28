// Gorky Deathmatch Game, All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	USTUHealthComponent();

	float GetHealth() const;

	UFUNCTION()
	bool IsDead() const;

	FOnHealthChanged OnHealthChanged;
	FOnDeath OnDeath;

	float Health = 0.0f;

  protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1000.0"), Category = "Health")
	float MaxHealth = 100.0f;

	virtual void BeginPlay() override;

  private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void ResolveTypeDamage(const UDamageType* DamageType);

	void TakeDamage(float Damage);

	void HealUpdate();

	void SetHealth(float NewHealthValue);

	FTimerHandle HealTimerHandle;
};
