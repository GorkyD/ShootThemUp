// Gorky Deathmatch Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "STUFireDamageType.h"
#include "STUIceDamageType.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	TakeDamage(Damage);
}

void USTUHealthComponent::TakeDamage(float Damage)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld())
		return;

	SetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}
void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::ResolveTypeDamage(const UDamageType* DamageType)
{
	if (DamageType)
	{
		if (DamageType->IsA<USTUFireDamageType>())
		{
			UE_LOG(LogHealthComponent, Display, TEXT("So hooooooooot !!!!!"))
		}
		else if (DamageType->IsA<USTUIceDamageType>())
		{
			UE_LOG(LogHealthComponent, Display, TEXT("So coooooold !!!!!"))
		}
	}
}

void USTUHealthComponent::SetHealth(float NewHealthValue)
{
	Health = FMath::Clamp(NewHealthValue, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

float USTUHealthComponent::GetHealth() const
{
	return Health;
}

bool USTUHealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}