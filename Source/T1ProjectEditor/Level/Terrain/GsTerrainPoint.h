// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GsTerrainPoint.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECTEDITOR_API UGsTerrainPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGsTerrainPoint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransfo,	ETeleportType Teleport) override;

public:	
	// Called every frame	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};