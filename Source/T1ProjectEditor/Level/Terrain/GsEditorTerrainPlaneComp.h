// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GsEditorTerrainPlaneComp.generated.h"

class AGsEditorTerrainShape;
class UGsEditorTerrainPillarComp;

//----------------------------------------------------------------
// AGsEditorBaseTerrain의 포인트 사이가 연결됐다는 것을 표현하기 위한 평면
//----------------------------------------------------------------
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECTEDITOR_API UGsEditorTerrainPlaneComp : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsEditorPillarComp")
		AGsEditorTerrainShape* _Parent;

private:
	UPROPERTY()
		UGsEditorTerrainPillarComp* _Start;
	UPROPERTY()
		UGsEditorTerrainPillarComp* _End;

public:
	// Sets default values for this component's properties
	UGsEditorTerrainPlaneComp();
	void Draw(UGsEditorTerrainPillarComp* in_start, UGsEditorTerrainPillarComp* in_end, float in_height, FColor in_outerColor, FColor in_insdieColor);

private:
	void DrawPlane(int32 in_section, FVector in_v1, FVector in_v2, FVector in_v3, FVector in_v4, bool in_clockOrient, FColor in_color);
	void SetPlaneMaterial(FColor in_color, int32 in_section);
	void SetNoCollision();

};
