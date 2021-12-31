// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AnyaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOLAR_SEAS_API AAnyaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAnyaPlayerController();

protected:

	virtual void BeginPlay() override;


private:

	// Ref to HUD overlay blueprint class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDOverlayClass;
	// Var to hold HUDOverlay widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;
};
