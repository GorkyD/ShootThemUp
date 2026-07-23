// Gorky Deathmatch Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUWeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()

  public:
	virtual void DrawHUD() override;

  protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	virtual void BeginPlay() override;

  private:
	void DrawCrossHair(EWeaponType WeaponType, float Spread);
	void DrawRiffleScopeCrossHair(FVector2D Center, FLinearColor LineColor, float Spread);
	void DrawLauncherScopeCrossHair(FVector2D Center, FLinearColor LineColor);

	USTUWeaponComponent* WeaponComponent;
};
