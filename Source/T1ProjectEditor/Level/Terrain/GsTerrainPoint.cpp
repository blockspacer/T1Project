// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrainPoint.h"

// Sets default values for this component's properties
UGsTerrainPoint::UGsTerrainPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsOnUpdateTransform = true;
	// ...
}


// Called when the game starts
void UGsTerrainPoint::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGsTerrainPoint::OnUpdateTransform(EUpdateTransformFlags UpdateTransfo, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransfo, Teleport);

	UE_LOG(LogTemp, Log, TEXT("Point"));
}

// Called every frame
void UGsTerrainPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
