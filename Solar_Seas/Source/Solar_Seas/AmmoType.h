// Copyright © 2022 Kdean Games. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_9mm UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "Assult Rife"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX"),
};