// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Rider_Unreal_Test/Rider_Unreal_TestCharacter.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class RIDER_UNREAL_TEST_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	ARider_Unreal_TestCharacter* PlayerCharacter;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
