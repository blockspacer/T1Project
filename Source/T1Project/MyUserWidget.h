// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

		

public:
	UMyUserWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void TestUI();


	UPROPERTY(EditAnywhere)
		int32 Time;


	
};