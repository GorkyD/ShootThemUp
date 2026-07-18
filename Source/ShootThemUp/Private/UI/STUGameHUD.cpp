// Gorky Deathmatch Game, All Rights Reserved.

#include "UI/STUGameHUD.h"

#include "Engine/Canvas.h"

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (const auto PlayerHudWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass)) PlayerHudWidget->AddToViewport();
}

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	// DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
	const TInterval Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const FLinearColor LineColor = FLinearColor::Green;

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}