// Fill out your copyright notice in the Description page of Project Settings.


#include "Rider_Unreal_Test/UserInterface/InventoryPanel.h"

#include "ItemDragDropOperation.h"
#include "Rider_Unreal_Test/Rider_Unreal_TestCharacter.h"
#include "Rider_Unreal_Test/Components/InventoryComponent.h"
#include "Rider_Unreal_Test/UserInterface/InventoryItemSlot.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ARider_Unreal_TestCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryTotalWeight(),
		InventoryReference->GetWeightCapacity()));

	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryContents().Num(),
		InventoryReference->GetSlotsCapacity()));
}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			if (UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass))
			{
				ItemSlot->SetItemReference(InventoryItem);
				InventoryPanel->AddChildToWrapBox(ItemSlot);
			}
		}

		SetInfoText();
	}
}



bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry,
								   const FDragDropEvent& InDragDropEvent,
								   UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."));

		// returning true will stop the drop operation at this widget
		return true;
	}

	// returning false will cause the drop operation to fall through to underlying widgets (if any)
	return false;
}

