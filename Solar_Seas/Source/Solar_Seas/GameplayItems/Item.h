// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SOLAR_SEAS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called with overlapping area sphere
	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// Called when end overlapping sphere.
	UFUNCTION()
	void OnSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Sets ActiveStars array based on rarity
	void SetActiveStars();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Skeletal mesh for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties|Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ItemMesh;
	// LineTrace collides with box to show HUB widgets.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties|Hit Box", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;
	// Popup widget for when player looks at item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties|Widget", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;
	// Enable item trace on overlap
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties|Sphere", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;
	// Item Name on Pickup widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties|Name", meta = (AllowPrivateAccess = "true"))
	FString ItemName;
	// Item count (ammo, etc)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties|Count", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;
	// Number of stars in the item widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties|Rarity", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties|Stars", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }

};
