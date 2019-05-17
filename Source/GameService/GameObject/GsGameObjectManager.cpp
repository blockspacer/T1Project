﻿#include "GsGameObjectManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameService.h"

#include "GameObject/ObjectClass/GsGameObjectLocal.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "../Class/GsSpawn.h"


AGsGameObjectManager::~AGsGameObjectManager()
{
	GSLOG(Warning, TEXT("~AGsGameObjectManager()"));
}


void AGsGameObjectManager::Initialize()
{
	TGsSingleton::InitInstance(this);

	Spawns.Empty();
	AddSpawns.Empty();
	RemoveSpawns.Empty();
	TypeSpawns.Empty(EGsGameObjectTypeALLCount);

	//타입별 빈공간 생성
	for (auto el : EGsGameObjectTypeALL)
	{
		TypeSpawns.Emplace(el);
	}
	
}

void AGsGameObjectManager::Finalize()
{
	for (auto el : Spawns)
	{
		el->Finalize();
	}

	TypeSpawns.Empty();
	AddSpawns.Empty();
	RemoveSpawns.Empty();
	Spawns.Empty();
}

void AGsGameObjectManager::Update()
{
	//대상 리스트 제거
	UpdateRemoveGameObject();
	//대상 추가
	UpdateAddGameObject();

	//스폰 오브젝트 업데이트
	for (auto el : Spawns)
	{
		el->Update(1.0);
	}
}


UGsGameObjectBase* AGsGameObjectManager::FindObject(AActor* Actor, EGsGameObjectType Type)
{
	if (Type == EGsGameObjectType::Base)
	{
		if (auto findobj = Spawns.FindByPredicate([=](UGsGameObjectBase* el)
		{
			AActor* a = el->GetActor();
			return NULL != a && a == Actor;
		}))
		{
			return *findobj;
		}
	}
	else
	{
		auto list = TypeSpawns[Type];
		if (auto findobj = list.FindByPredicate([=](UGsGameObjectBase* el)
		{
			AActor* a = el->GetActor();
			return NULL != a && a == Actor;
		}))
		{
			return *findobj;
		}
	}

	return NULL;
}

UGsGameObjectBase* AGsGameObjectManager::FindObject(EGsGameObjectType Type)
{
    return TypeSpawns[Type].Num() > 0 ? TypeSpawns[Type].Top() : NULL;
}

TArray<UGsGameObjectBase*> AGsGameObjectManager::FindObjects(EGsGameObjectType Type)
{
	return TypeSpawns[Type];
}

UGsGameObjectBase* AGsGameObjectManager::SpawnPlayer(UClass* Uclass, const FVector& Pos, const FRotator& Rot)
{
	auto local = NewObject<UGsGameObjectLocal>();
	local->Initialize();
	if (auto actor = TGsSpawn::BPClass(GetWorld(), Uclass, Pos, Rot))
	{
		local->ActorSpawned(actor);
		AddSpawns.Emplace(local);
		actor->OnDestroyed.AddDynamic(this, &AGsGameObjectManager::CallbackActorDeSpawn);
	}
	return local;
}

//[Todo] Class UCharacterMovementComponent
//void UCharacterMovementComponent::FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult) const
//내부 구현을 참고 하여 개선한다.
UGsGameObjectBase* AGsGameObjectManager::SpawnNpc(UClass* Uclass, const FVector& Pos, const FRotator& Rot)
{
	auto npc = NewObject<UGsGameObjectNonPlayer>();
	npc->Initialize();

	FVector rayOri = Pos + FVector(0.f, 0.f, 1000.f);
	FVector rayDes = rayOri + FVector(0.f, 0.f, -1.f)  * 1200.f;

	FHitResult HitResult;
	TArray<AActor*> actorsToIgnore;

	//생성도 되기전에 얻어오는게 맞는지 모르것음...
	if (AActor* castActor = Uclass->GetDefaultObject<AActor>())
	{
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), rayOri, rayDes, UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false, actorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red);
		if (HitResult.bBlockingHit)
		{
			FVector location = HitResult.Location + FVector(0.f, 0.f, 0.5f);
			if (UCapsuleComponent* Capshule = castActor->FindComponentByClass<UCapsuleComponent>())
			{
				location.Z += Capshule->GetUnscaledCapsuleHalfHeight();
			}

			if (auto actor = TGsSpawn::BPClass(GetWorld(), Uclass, Pos, Rot))
			{
				npc->ActorSpawned(actor);
				AddSpawns.Emplace(npc);

                //액터 자동 소멸 콜백을 연결하여 관리 대상 동기화를 맞춤
				actor->OnDestroyed.AddDynamic(this, &AGsGameObjectManager::CallbackActorDeSpawn);
			}
		}
	}

	return npc;
}

UGsGameObjectBase* AGsGameObjectManager::SpawnProjectile(UClass* Uclass, const FVector& Pos, const FRotator& Rot)
{
	auto projectile = NewObject<UGsGameObjectProjectile>();
	projectile->Initialize();

	if (auto actor = TGsSpawn::BPClass(GetWorld(), Uclass, Pos, Rot))
	{
		AddSpawns.Emplace(projectile);
		projectile->ActorSpawned(actor);
		actor->OnDestroyed.AddDynamic(this, &AGsGameObjectManager::CallbackActorDeSpawn);


		//충돌 처리
		if (UPrimitiveComponent* collider = actor->FindComponentByClass<UPrimitiveComponent>())
		{
			collider->OnComponentHit.AddDynamic(this, &AGsGameObjectManager::CallbackCompHit);
		}
	}
	return projectile;
}

void AGsGameObjectManager::DespawnObject(UGsGameObjectBase* Despawn)
{
	if (Spawns.Contains(Despawn))
	{
		//액터 소멸
		GetWorld()->DestroyActor(Despawn->GetActor());

		//액터 소멸시 일단 관리대상 에서 제거
		RemoveSpawns.Emplace(Despawn);
	}
}

void AGsGameObjectManager::UpdateAddGameObject()
{
	//대상 추가
	if (0 < AddSpawns.Num())
	{
		for (auto el : AddSpawns)
		{
			uint8 key = el->GetObjectTypeMask();
			for (auto el2 : EGsGameObjectTypeALL)
			{
				if (CHECK_FLAG_TYPE(key, el2))
				{
					TypeSpawns[el2].Emplace(el);
				}
			}
			Spawns.Emplace(el);
		}
		AddSpawns.Empty();
	}
}

void AGsGameObjectManager::UpdateRemoveGameObject()
{
	if (0 < RemoveSpawns.Num())
	{
		for (auto el : RemoveSpawns)
		{
			uint8 key = el->GetObjectTypeMask();
			for (auto el2 : EGsGameObjectTypeALL)
			{
				if (CHECK_FLAG_TYPE(key, el2))
				{
					TypeSpawns[el2].RemoveSwap(el);
				}
			}

			Spawns.RemoveSwap(el);
		}
		RemoveSpawns.Empty();
	}
}

void AGsGameObjectManager::CallbackCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		auto hitObj = FindObject(HitComponent->GetOwner(), EGsGameObjectType::Projectile);
		//피격 알림
		if (auto findObj = FindObject(OtherActor, EGsGameObjectType::NonPlayer))
		{
			findObj->OnHit(hitObj);
		}

		if (hitObj)
		{
			//충돌 객체 성향에 따라 Destory여부 설정하기
			//현재는 무조건 삭제
			DespawnObject(hitObj);
		}
	}
}

void AGsGameObjectManager::CallbackActorDeSpawn(AActor* Despawn)
{
	//관리 대상인가 찾음
	if (auto findObj = FindObject(Despawn))
	{
		RemoveSpawns.Emplace(findObj);
	}
}