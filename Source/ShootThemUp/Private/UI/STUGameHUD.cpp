// Gorky Deathmatch Game, All Rights Reserved.

#include "UI/STUGameHUD.h"

#include "STULauncherWeapon.h"
#include "STURiffleWeapon.h"
#include "STUWeaponComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/Pawn.h"

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (const auto PlayerHudWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass)) PlayerHudWidget->AddToViewport();
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController()) WeaponComponent = PlayerController->GetPawn()->FindComponentByClass<USTUWeaponComponent>();

	check(WeaponComponent);
}

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrossHair(WeaponComponent->GetCurrentWeaponType(), WeaponComponent->GetCurrentSpread());
}

void ASTUGameHUD::DrawCrossHair(const TSubclassOf<ASTUBaseWeapon>& WeaponType, float Spread)
{
	const FVector2D Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
	const FLinearColor LineColor = FLinearColor::Green;

	if (WeaponType->IsChildOf(ASTULauncherWeapon::StaticClass()))
	{
		DrawLauncherScopeCrossHair(Center, LineColor);
	}
	else if (WeaponType->IsChildOf(ASTURiffleWeapon::StaticClass()))
	{
		DrawRiffleScopeCrossHair(Center, LineColor, Spread);
	}
}

void ASTUGameHUD::DrawRiffleScopeCrossHair(FVector2D Center, FLinearColor LineColor, float Spread)
{
	constexpr float Thickness = 1.f;
	constexpr float Length = 10.f;

	const float SpreadPixels = Spread * 5.f;

	DrawLine(Center.X - SpreadPixels - Length, Center.Y, Center.X - SpreadPixels, Center.Y, LineColor, Thickness);
	DrawLine(Center.X + SpreadPixels, Center.Y, Center.X + SpreadPixels + Length, Center.Y, LineColor, Thickness);
	DrawLine(Center.X, Center.Y - SpreadPixels - Length, Center.X, Center.Y - SpreadPixels, LineColor, Thickness);
	DrawLine(Center.X, Center.Y + SpreadPixels, Center.X, Center.Y + SpreadPixels + Length, LineColor, Thickness);
}

void ASTUGameHUD::DrawLauncherScopeCrossHair(FVector2D Center, FLinearColor LineColor)
{
	constexpr float HalfSize = 20.f;
	constexpr float Thickness = 1.f;
	constexpr float DotSize = 3.f;

	DrawLine(Center.X - HalfSize, Center.Y - HalfSize, Center.X + HalfSize, Center.Y - HalfSize, LineColor, Thickness);
	DrawLine(Center.X + HalfSize, Center.Y - HalfSize, Center.X + HalfSize, Center.Y + HalfSize, LineColor, Thickness);
	DrawLine(Center.X + HalfSize, Center.Y + HalfSize, Center.X - HalfSize, Center.Y + HalfSize, LineColor, Thickness);
	DrawLine(Center.X - HalfSize, Center.Y + HalfSize, Center.X - HalfSize, Center.Y - HalfSize, LineColor, Thickness);
	DrawLine(Center.X - DotSize, Center.Y, Center.X + DotSize, Center.Y, LineColor, Thickness);
	DrawLine(Center.X, Center.Y - DotSize, Center.X, Center.Y + DotSize, LineColor, Thickness);
}