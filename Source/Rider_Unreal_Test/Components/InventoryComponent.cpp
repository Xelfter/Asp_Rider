// Fill out your copyright notice in the Description page of Project Settings.


#include "Rider_Unreal_Test/Components/InventoryComponent.h"

#include "ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(): InventoryTotalWeight(0), InventorySlotsCapacity(0),
                                            InventoryWeightCapacity(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}



UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = 
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}))
	{
		return *Result;
	}

	return nullptr;
}


int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount) const
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt(
		(GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());

	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(const UItemBase* StackableItem, const int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}



void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// Check if the input item has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."),
			InputItem->TextData.Name));
	}

	// Will the item weight overflow weight capacity
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."),
			InputItem->TextData.Name));
	}

	// Adding one more item would overflow slot capacity
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add a single{0} to the inventory. All inventory slots are full."),
			InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1);
	return FItemAddResult::AddedAll(1, FText::Format(
			FText::FromString("Successfully added {0} {1} to the inventory."),
			InputItem->TextData.Name));
}
int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
    // Validate the requested amount and ensure the item's weight is valid
    if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
    {
        // Invalid item data; cannot proceed
        return 0;
    }

    // Track the remaining amount of items to be added to the inventory
    int32 AmountToDistribute = RequestedAddAmount;

    // Check if the item is already in the inventory and can stack (not full stack)
    UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

    // Distribute the requested amount across existing stacks in the inventory
    while (ExistingItemStack)
    {
        // Calculate how many items are needed to fill the next stack
        const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);

        // Calculate how many items can be added without exceeding the weight limit
        const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

        // If weight capacity allows adding more items
        if (WeightLimitAddAmount > 0)
        {
            // Add items to the existing stack
            ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
            // Update the inventory's total weight
            InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

            // Update the remaining amount to distribute
            AmountToDistribute -= WeightLimitAddAmount;
            ItemIn->SetQuantity(AmountToDistribute);

            // If the inventory's weight capacity is exceeded
            if (InventoryTotalWeight >= InventoryWeightCapacity)
            {
                // Notify that the inventory was updated and return the amount added
                OnInventoryUpdated.Broadcast();
                return RequestedAddAmount - AmountToDistribute;
            }
        }
        else if (WeightLimitAddAmount <= 0)
        {
            // If no more items can be added due to weight, return the amount added
            if (AmountToDistribute != RequestedAddAmount)
            {
            	//Multiple stacks with weight limit reached
                OnInventoryUpdated.Broadcast();
                return RequestedAddAmount - AmountToDistribute;
            }
            return 0;
        }

        // If all requested items have been distributed
        if (AmountToDistribute <= 0)
        {
        	
            OnInventoryUpdated.Broadcast();
            return RequestedAddAmount;
        }

        // Check if there are additional matching stacks to distribute item
        ExistingItemStack = FindMatchingItem(ItemIn);
    }

    // If no existing stacks are found, add items to a new stack
    if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
    {
        // Calculate how many items can be added to a new stack without exceeding weight
        const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

        if (WeightLimitAddAmount > 0)
        {
            // If only part of the items can be added due to weight
            if (WeightLimitAddAmount < AmountToDistribute)
            {
                // Update remaining items and add a partial stack
                AmountToDistribute -= WeightLimitAddAmount;
                ItemIn->SetQuantity(AmountToDistribute);

                AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
                return RequestedAddAmount - AmountToDistribute; // Return the amount added
            }

            // Add the remaining items as a new stack
            AddNewItem(ItemIn, AmountToDistribute);
            return RequestedAddAmount;
        }
    }

    // If no items could be added due to weight or slot limitations
    OnInventoryUpdated.Broadcast();
    return RequestedAddAmount - AmountToDistribute;
}


FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// Handle non-stackable items
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// Handle stackable items
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(
				InitialRequestedAddAmount,
				FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(
				StackableAmountAdded,
				FText::Format(FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->TextData.Name));
		}
	}
	
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		//if item si already a copy or world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		//use when splitting or drag from other inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}