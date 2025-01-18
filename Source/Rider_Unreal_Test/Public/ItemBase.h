#pragma once

#include "CoreMinimal.h"
#include "Rider_Unreal_Test/Data/ItemDataStructs.h"
#include "Rider_Unreal_Test/Rider_Unreal_TestCharacter.h"
#include "ItemBase.generated.h"


UCLASS()	
class RIDER_UNREAL_TEST_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	//UPROPERTY()
	//UInventoryComponent* OwningInventory;
	
	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (UIMin=1, UIMax=100))
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
	

	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float IsFullStack() const { return Quantity * NumericData.MaxStackSize; };
	
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);
	
	UFUNCTION(Category = "Item")
	virtual void Use(ARider_Unreal_TestCharacter* Character);

	
protected:
	bool operator ==(const FName& OtherID) const 
	{
		return ID == OtherID;
	}
};


//UCLASS()
//class RIDER_UNREAL_TEST_API UItemBase : public UObject
//{
//	GENERATED_BODY()
//};
