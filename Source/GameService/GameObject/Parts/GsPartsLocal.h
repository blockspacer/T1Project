﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsPartsBase.h"
#include "Data/GsPartsDataBase.h"

class UGsGameObjectLocal;
class USkeletalMeshComponent;

/**
 * 파츠 담당 클래스
 * 추후 실제 Mesh 교체 로직과 Parts Data 입출력 로직을 구분하여 
 * ACharacter or APawn 파생 클래스에 Mesh교체 부분을  FGameObjectBase 파생클래스에  Parts Data 입출력 로직을 처리 해야 할듯
 */
class GAMESERVICE_API FGsPartsLocal : public FGsPartsBase
{
	typedef FGsPartsBase Super;

public:
	FGsPartsLocal();
	virtual ~FGsPartsLocal();

	virtual void Initialize(UGsGameObjectBase* Owner) override;

protected:
	virtual void Attached() override;
	virtual void Detached() override;

private:
	//테스트용
	//파츠 병합을 MeshMerge타입과 MasterPose타입으로 분류한다.
	void MasterPose(USkeletalMeshComponent* SkeletaMesh);
	USkeletalMesh* MergeParts() const;

	void ToMergeParams(const TArray<FSkelMeshMergeSectionMapping_BP>& InSectionMappings,
		TArray<FSkelMeshMergeSectionMapping>& OutSectionMappings);
	void ToMergeParams(const TArray<FSkelMeshMergeUVTransformMapping>& InUVTransformsPerMesh,
		TArray<FSkelMeshMergeUVTransforms>& OutUVTransformsPerMesh);

	USkeletalMesh* MergeToParams(const FSkeletalMeshMergeParams& Params);

private:
	UGsGameObjectLocal* Local;
	UActorComponent* ActorComponent;
};
