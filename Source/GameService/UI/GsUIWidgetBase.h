// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIParameter.h"
#include "GsUIEventInterface.h"
#include "GsUIWidgetBase.generated.h"


/**
 * GsUIManager에서 관리받는 위젯의 기본클래스. GsUIWindow, GsUIPopup, GsUITray 를 사용할 것.
 * 개선: abstract 클래스로 만들면 좋겠다
 */
UCLASS()
class GAMESERVICE_API UGsUIWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGsUIWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void RemoveFromParent() override;
	virtual void BeginDestroy() override;

	// 스택 관리되는 UI인가
	virtual bool IsStackUI() const { return false; }
	
	// 윈도우 타입(화면을 다 가리는 타입. GsUIWindow 상속객체)인가
	virtual bool IsWindow() const { return false; }

	// 레벨 로드 시 파괴되지 않는 UI인가
	virtual bool IsNondestructiveWidget() const { return bNotDestroy; }

	// 중복 생성이 가능한 위젯
	virtual bool CanMultipleInstance() const { return bCanMultipleInstance; }

	// UIController에서 캐싱하고 있는 위젯 
	bool IsCachedWidget() const { return bIsCachedWidget; }

	UFUNCTION(BlueprintCallable, Category = "GsManaged")
	virtual void Close();

	// FName을 구분자로 쓸 수 있는 메시지를 보냄
	UFUNCTION(BlueprintNativeEvent, Category = "GsManaged", meta = (DisplayName = "OnMessage"))
	void OnMessage(FName InKey, UGsUIParameter* InParam = nullptr);
	virtual void OnMessage_Implementation(FName InKey, UGsUIParameter* InParam = nullptr);

	UFUNCTION(BlueprintCallable, Category = "GsManaged")
	class UGsUIManager* GetUIManager();

protected:
	//스택에 푸쉬할 때 부르며, 초기화 데이터를 넘길 수 있음.
	//주의: OnInitialized 뒤에 불리지만 Construct 보다 전에 불리므로 Slate 세팅과 관련된 처리를 여기서 하면 안됨.
	UFUNCTION(BlueprintNativeEvent, Category = "GsManaged", meta = (DisplayName = "OnPush"))
	void OnPush(UGsUIParameter* InParam = nullptr);
	virtual void OnPush_Implementation(UGsUIParameter* InParam = nullptr);

	// Window < Popup < Tray 뎁스 보장을 위한 값. 자손 클래스에서 값 부여(Window: 10, Popup: 100, Tray: 500)
	virtual int32 GetManagedDefaultZOrder() const { return 0; }
	virtual int32 GetManagedZOrder() const;

	// 현재 Visibility 상태를 백업 후 Hidden 상태로 전환
	virtual void Hide();
	
	// 이전 백업해둔 상태로 돌림
	virtual void Unhide();

	// 주의: 초기화, 파괴시에만 사용할 것. 초기화 시 bNotDestroy 에 의해 설정. 씬전환 시 파괴처리여부.
	void SetEnableAutoDestroy(bool bInEnableAutoDestroy);
	
protected:
	// Window, Popup, Tray 순서보장 관리를 받지 않고 강제로 세팅하고 싶을 경우 true로 설정
	UPROPERTY(EditDefaultsOnly, Category = "GsManaged")
	bool bNotUseManagedZOrder;

	// 주의: 최적화를 위해 가능한 쓰지 않도록 하는 것이 좋겠음.
	// 뎁스 조절을 위해 더해질 값. 
	UPROPERTY(EditDefaultsOnly, Category = "GsManaged")
	int32 AddZOrder;

	// 레벨 전환 시 파괴할 오브젝트인가
	UPROPERTY(EditDefaultsOnly, Category = "GsManaged")
	bool bNotDestroy;

	UPROPERTY(EditDefaultsOnly, Category = "GsManaged")
	bool bCanMultipleInstance;

private:
	// 스택 중복 처리를 막는다
	bool bStackProcessed;
	ESlateVisibility BackupVisibility;
	bool bIsCachedWidget;
	bool bEnableAutoDestroy;

	friend class UGsUIController;
	friend class UGsUIControllerNotDestroy;

	/*
	// 이하는 UserWidget에서 제공되는 함수로, 용도에 맞게 상속받아 사용할 것.

	// CreateWidget 시 한 번 불린다. 한 번만 수행해야 할 항목 처리 시 사용. (BP이벤트: OnInitialized)
	virtual void NativeOnInitialized() override;
	
	// AddToViewport 직후에 불림. (BP이벤트: PreConstruct)
	virtual void NativePreConstruct() override;
	
	// Slate 세팅이 완료되면 불린다. 일반적인 초기세팅에 사용. (BP이벤트: Construct) 
	// 주의: 유니티의 OnEnable 같지만 SetVisible에 영향받지 않음에 유의
	virtual void NativeConstruct() override;

	// RemoveFromParent된 후 불린다. (BP이벤트: Destruct) 
	// 주의: 유니티의 OnDisable 같지만 SetVisible에 영향받지 않음에 유의
	virtual void NativeDestruct() override;

	// RefCount가 0이되어 실제 파괴될 때 불린다. 별도 BP이벤트는 제공하지 않는다.
	virtual void BeginDestroy() override;
	*/
};
