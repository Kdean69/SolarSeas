// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Skeletal mesh for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items|Item Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ItemMesh;
	// LineTrace collides with box to show HUB widgets.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items|Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;


public:

};
