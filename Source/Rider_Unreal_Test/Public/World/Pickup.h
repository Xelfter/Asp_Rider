// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Rider_Unreal_Test/Rider_Unreal_TestCharacter.h"
#include "Pickup.generated.h"

class UDataTable;
class UItemBase;

UCLASS()
class RIDER_UNREAL_TEST_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	APickup();

	
	virtual void BeginFocus() override;
	
	virtual void EndFocus() override;
	
	

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }

protected:

	
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	
	
	virtual void BeginPlay() override;
	

	virtual void Interact(ARider_Unreal_TestCharacter* PlayerCharacter) override;
	
	void UpdateInteractableData();

	
	void TakePickup(const ARider_Unreal_TestCharacter* Taker);

#if WITH_EDITOR	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	
};
