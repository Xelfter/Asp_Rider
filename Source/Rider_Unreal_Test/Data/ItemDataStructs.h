#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"


UENUM()
enum class EItemQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legandary UMETA(DisplayName = "Legandary"),
};

UENUM()
enum class EItemType : uint8 
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditeAnywhere)
	float ArmorRating;

	UPROPERTY(EditeAnywhere)
	float DamageValue;

};

USTRUCT()
struct FItemTextData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditeAnywhere)
	FText Name;

	UPROPERTY(EditeAnywhere)
	FText Description;

	UPROPERTY(EditeAnywhere)
	FText InetractionText;

	UPROPERTY(EditeAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditeAnywhere)
	float Weight;

	UPROPERTY(EditeAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditeAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditeAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditeAnywhere)
	UStatisticMesh* Mesh;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ID;

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


};