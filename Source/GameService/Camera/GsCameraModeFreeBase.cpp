#include "GsCameraModeFreeBase.h"
#include "GameService.h"
#include "GsCameraModeManager.h"
#include "EngineMinimal.h"

#include "../GameObject/ObjectClass/GsGameObjectLocal.h"
#include "../GameObject/ActorExtend/GsLocalCharacter.h"
#include "../GameObject/Input/GsInputBindingLocalPlayer.h"
#include "../GameObject/Movement/GsMovementBase.h"

GsCameraModeFreeBase::GsCameraModeFreeBase()
{
	GSLOG(Warning, TEXT("GsCameraModeFreeBase constructor"));
}
GsCameraModeFreeBase::~GsCameraModeFreeBase()
{
	GSLOG(Warning, TEXT("GsCameraModeFreeBase destructor"));
}

void GsCameraModeFreeBase::Enter()
{
	GSLOG(Warning, TEXT("GsCameraModeFreeBase Enter"));
}

void GsCameraModeFreeBase::Exit()
{
	GSLOG(Warning, TEXT("GsCameraModeFreeBase Exit"));

}

void GsCameraModeFreeBase::Update()
{
	GSLOG(Warning, TEXT("GsCameraModeFreeBase Update"));
}
// ���� ��ȯ(���ڷ� ĳ����)
void GsCameraModeFreeBase::Enter(UGsGameObjectLocal* In_char, GsCameraModeManager* In_mng)
{
	if (In_char == nullptr)
	{
		return;
	}

	GsCameraModeBase::Enter(In_char, In_mng);

	AGsLocalCharacter* localChar = In_char->GetLocalCharacter();
	if (localChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr"));
		return;
	}

	UGsInputBindingLocalPlayer* inputBinding = Cast<UGsInputBindingLocalPlayer>(localChar->GetInputBinder());

	if (inputBinding == nullptr)
	{
		GSLOG(Error, TEXT("inputBinding == nullptr"));
		return;
	}

	// ���ε� ó��
	// ���� ĸ�ĸ� ���۷���(&)�� �ϰ� ������ ������ �ٸ��ּҸ� �Ѱ��༭
	// ���� ȣ��� �̻� ��Ȳ �߻���....
	// �����ʹ� ����(=)�� ĸ���ؾ���....
	inputBinding->FunctionMoveForward = [=]() {MoveForward(In_char); };
	inputBinding->FunctionMoveBackward = [=]() {MoveBackward(In_char); };
	inputBinding->FunctionMoveLeft = [=]() {MoveLeft(In_char); };
	inputBinding->FunctionMoveRight = [=]() {MoveRight(In_char); };

	inputBinding->FunctionLookUp = [=](float val) {LookUp(val, In_char); };
	inputBinding->FunctionTurn = [=](float val) {Turn(val, In_char); };

	localChar->GetSpringArm()->bUsePawnControlRotation = true;
	localChar->GetSpringArm()->bInheritPitch = true;
	localChar->GetSpringArm()->bInheritRoll = true;
	localChar->GetSpringArm()->bInheritYaw = true;
	localChar->GetSpringArm()->bDoCollisionTest = true;
	localChar->bUseControllerRotationYaw = false;

	localChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	localChar->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	localChar->GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);



	GSLOG(Warning, TEXT("GsCameraModeFreeBase  Enter"));
}
// ���� ����(���ڷ� ĳ����)
void GsCameraModeFreeBase::Exit(UGsGameObjectLocal* In_char)
{
	if (In_char == nullptr)
	{
		return;
	}

	GsCameraModeBase::Exit(In_char);

	AGsLocalCharacter* localChar = In_char->GetLocalCharacter();
	if (localChar == nullptr)
	{
		GSLOG(Error, TEXT("localChar == nullptr"));
		return;
	}

	UGsInputBindingLocalPlayer* inputBinding = Cast<UGsInputBindingLocalPlayer>(localChar->GetInputBinder());

	if (inputBinding == nullptr)
	{
		GSLOG(Error, TEXT("inputBinding == nullptr"));
		return;
	}

	// ���ε� ����
	inputBinding->FunctionMoveForward = nullptr;
	inputBinding->FunctionMoveBackward = nullptr;
	inputBinding->FunctionMoveLeft = nullptr;
	inputBinding->FunctionMoveRight = nullptr;
	inputBinding->FunctionLookUp = nullptr;
	inputBinding->FunctionTurn = nullptr;


	GSLOG(Warning, TEXT("GsCameraModeFreeBase  exit"));
}
// ��������(���ڷ� ĳ����)
void GsCameraModeFreeBase::Update(UGsGameObjectLocal* In_char, float In_deltaTime, GsCameraModeManager* In_mng)
{
	GsCameraModeBase::Update(In_char, In_deltaTime, In_mng);

}

// ��,�Ʒ� �̵� ó��
void GsCameraModeFreeBase::UpDown(float NewAxisValue, UGsGameObjectLocal* In_char)
{



	//GSLOG(Warning, TEXT("UpDown axisVal: %f"), NewAxisValue);
}
// ��,�� �̵� ó��
void GsCameraModeFreeBase::LeftRight(float NewAxisValue, UGsGameObjectLocal* In_char)
{

	//GSLOG(Warning, TEXT("LeftRight axisVal: %f"), NewAxisValue);
}

// �� �̵� ó��
void GsCameraModeFreeBase::MoveForward(UGsGameObjectLocal* In_char)
{
	if (In_char == nullptr)
	{
		return;
	}

	if (auto movement = In_char->GetMovement())
	{
		FVector dir = FRotationMatrix(In_char->GetLocalCharacter()->Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		movement->Move(dir, EGsGameObjectMoveDirType::Forward, 10.0f);
	}
}
// �� �̵� ó��
void GsCameraModeFreeBase::MoveBackward(UGsGameObjectLocal* In_char)
{
	if (In_char == nullptr)
	{
		return;
	}
	if (auto movement = In_char->GetMovement())
	{
		FVector dir = FRotationMatrix(In_char->GetLocalCharacter()->Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		movement->Move(dir, EGsGameObjectMoveDirType::Backward, -5.f);
	}
}
// �� �̵� ó��
void GsCameraModeFreeBase::MoveLeft(UGsGameObjectLocal* In_char)
{
	if (In_char == nullptr)
	{
		return;
	}
	if (auto movement = In_char->GetMovement())
	{
		FVector dir = FRotationMatrix(In_char->GetLocalCharacter()->Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		movement->Move(dir, EGsGameObjectMoveDirType::SideStep, -5.f);
	}
}
// �� �̵� ó��
void GsCameraModeFreeBase::MoveRight(UGsGameObjectLocal* In_char)
{
	if (In_char == nullptr)
	{
		return;
	}
	if (auto movement = In_char->GetMovement())
	{
		FVector dir = FRotationMatrix(In_char->GetLocalCharacter()->Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		movement->Move(dir, EGsGameObjectMoveDirType::SideStep, 5.f);
	}
}

// ��, �Ʒ� ī�޶� ȸ��
void GsCameraModeFreeBase::LookUp(float NewAxisValue, UGsGameObjectLocal* In_char)
{

	if (In_char == nullptr)
	{
		return;
	}
	In_char->GetLocalCharacter()->AddControllerPitchInput(NewAxisValue);

}
// ��, �� ī�޶� ȸ��
void GsCameraModeFreeBase::Turn(float NewAxisValue, UGsGameObjectLocal* In_char)
{

	if (In_char == nullptr)
	{
		return;
	}
	In_char->GetLocalCharacter()->AddControllerYawInput(NewAxisValue);

}