// Copyright © 2021 Kdean Games. All Rights Reserved.


#include "AnyaPlayerController.h"
#include "Blueprint/UserWidget.h"

AAnyaPlayerController::AAnyaPlayerController()
{

}

void AAnyaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Check HUD overlay class
	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
		if (HUDOverlay)
		{
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}