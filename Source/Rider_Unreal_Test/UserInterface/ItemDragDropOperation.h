// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Rider_Unreal_Test/Components/InventoryComponent.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class RIDER_UNREAL_TEST_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;
};
