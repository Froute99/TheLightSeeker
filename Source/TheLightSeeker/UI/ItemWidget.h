// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELIGHTSEEKER_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void OnRegisterItem();

	UFUNCTION(BlueprintCallable)
	void OnUseItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	
};
