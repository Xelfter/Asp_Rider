// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Rider_Unreal_Test/Data/ItemDataStructs.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Rider_Unreal_Test/Rider_Unreal_TestCharacter.h"



UItemBase::UItemBase(): Quantity(0),
	  ID(NAME_None),
	  ItemType(EItemType::Consumable),
	  ItemQuality(EItemQuality::Common),
	  ItemStatistics({0.0f, 0.0f}),
	  NumericData({0.0f, 1, false}),
	  AssetData({nullptr, nullptr})
{
	
}

::UItemBase* ::UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;

	return ItemCopy;
}

void ::UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0,  NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
          
         
		//if (OwningInventory) 
		{
		//	if (Quantity <= 0) 
			{
			//	owningInventory->RemoveItem(this);
			}
		}
	}
};

void ::UItemBase::Use(ARider_Unreal_TestCharacter* Character)
{
	
}
