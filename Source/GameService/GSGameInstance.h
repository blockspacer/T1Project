// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "Class/GSTContainer.h"
#include "GameMode/GSFGameModeManager.h"
#include "Message/GSFMessageManager.h"
#include "Net/GSFNetManager.h"

#include "GSGameInstance.generated.h"

class GSIManager;

//-------------------------------------------------------------------------------
// 게임초기화 순서
//-------------------------------------------------------------------------------
// 1. 게임 앱의 초기화(PlayMode)	:	UGameInstance::Init
// 2. 레벨에 속한 액터의 초기화		:	AActor::PostInitailizeComponents
// 3. 플레이어의 로그인			:	AGameMode::PostLogin
// 4. 게임의 시작					:	AGameMode::StartPlay -> AActor::BeginPlay
//-------------------------------------------------------------------------------

UCLASS()
class GAMESERVICE_API UGSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	GSTArray<GSIManager> _manage;	
public:
	UGSGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;
};
