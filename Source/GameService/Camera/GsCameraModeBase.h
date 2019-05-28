#pragma once

#include "GsControlMode.h"
#include "../Class/GsState.h"


//----------------------------------------------------------------------
// 카메라 모드 기반
//----------------------------------------------------------------------

class GsCameraModeManager;
class GsCameraModeBase : public TGsState<EGsControlMode>
{
public :
	GsCameraModeBase() : TGsState<EGsControlMode>(EGsControlMode::Free) {}
	GsCameraModeBase(EGsControlMode In_mode) :TGsState<EGsControlMode>(In_mode) {}
	virtual ~GsCameraModeBase() {}
	virtual void Enter() override {};
	virtual void Exit() override {};
	virtual void Update() override {};

	// 상태 전환(인자로 캐릭터)
	virtual void Enter(ACharacter* In_char, GsCameraModeManager* In_mng);
	// 상태 종료(인자로 캐릭터)
	virtual void Exit(ACharacter* In_char);
	// 업데이터(인자로 캐릭터)
	virtual void Update(ACharacter* In_char, float In_deltaTime);

	// 위,아래 이동 처리
	virtual void UpDown(float NewAxisValue, ACharacter* In_char) {}
	// 좌,우 이동 처리
	virtual void LeftRight(float NewAxisValue, ACharacter* In_char) {}
	// 위, 아래 카메라 회전
	virtual void LookUp(float NewAxisValue, ACharacter* In_char) {}
	// 좌, 우 카메라 회전
	virtual void Turn(float NewAxisValue, ACharacter* In_char) {}
	// 줌인
	virtual void ZoomIn();
	// 줌아웃
	virtual void ZoomOut();
	// 다음 스텝 진행(매니저 넘김)
	virtual void NextStep(GsCameraModeManager* In_mng) {}

	// 줌아웃 강도
	const float ZoomInOutPower = 20.0f;

	// 스프링 암 거리 최대치
	const float ArmLengthMax = 1000.0f;
	// 스프링 암 거리 최소치
	const float ArmLengthMin = 300.0f;

	// 스프링 암 거리 보간 속도
	const float ArmLengthSpeed = 3.0f;

private:

	// 스프링 암 거리 목표값
	float _armLengthTo = 0.0f;
};