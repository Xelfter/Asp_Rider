#pragma once

#include "CoreMinimal.h"
#include "Rider_Unreal_Test/Data/ItemDataStructs.h"
#include "Rider_Unreal_Test/Rider_Unreal_TestCharacter.h"
#include "ItemBase.generated.h"

class UInventoryComponent;

UCLASS()	
class RIDER_UNREAL_TEST_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Item", meta = (UIMin=1, UIMax=100))
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;

	bool bIsCopy;
	bool bIsPickup;


	
	UItemBase();

	void ResetItemFlags();
	
	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float IsFullItemStack() const { return Quantity * NumericData.MaxStackSize; };
	
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);
	
	UFUNCTION(Category = "Item")
	virtual void Use(ARider_Unreal_TestCharacter* PlayerCharacter);

	
protected:
	bool operator ==(const FName& OtherID) const 
	{
		return this -> ID == OtherID;
	}
};


//UCLASS()
//class RIDER_UNREAL_TEST_API UItemBase : public UObject
//{
//	GENERATED_BODY()
//};
