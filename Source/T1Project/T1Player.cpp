// Fill out your copyright notice in the Description page of Project Settings.

#include "T1Player.h"
#include "T1Project.h"
#include "T1AnimInstance.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Delegate.h"
#include "ConstructorHelpers.h"


// Sets default values
AT1Player::AT1Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �ɸ��Ϳ��� ī�޶� �����մϴ�.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// ������ ���۳�Ʈ ����
	CharInfoData = CreateDefaultSubobject<UDataActorComponent>(TEXT("CHARINFO"));

	// ������Ʈ ���Ӱ��輳��
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	
	// ��ǥ����(�����ǥ�� �ʱⰪ�� �����մϴ�.)
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// ���̷�Ż �޽�����(����) 
	// ConstructorHelpers �ش�迭�� �Լ��� static���� �����ѰͿ� ����(������������ �迭 Ŭ������ �ν��Ͻ̵ɶ����� ȣ���ϴ°��� ����)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HERO(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_RaggedElite.SK_CharM_RaggedElite"));
	//ConstructorHelpers::FObjectFinder (������Ʈ �˻�)
	if (SK_HERO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_HERO.Object);
	}

	// �ִϸ��̼��� �÷����� ��带 �����Ѵ�.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// �ִϸ��̼� ��������Ʈ�� �ִ� �׷��� ������ ���� �����ϴ� �ɸ��� �ִϸ��̼� ������ ������Ű�µ� �̷��� �ý����� C++���� �ִ��ν��ϵ� Ŭ������ �����ȴ�
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARR_ANIM(TEXT("/Game/InfinityBladeWarriors/Character/WarriorAnimBP.WarriorAnimBP_C"));
	//ConstructorHelpers::FClassFinder  *���� ��������Ʈ ���� _C�� �ٿ��� �˻��� (Ŭ���� �˻�) : WarriorAnimBP_C
	if (WARR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARR_ANIM.Class);
	}

	// ī�޶� ��� �������� ������
	SetCameraControlMode(ECameraControlMode::DIABLO);

	// ī�޶� ���氣 ���� ����
	ArmLengthSpeed = 5.0f;
	ArmRotationSpeed = 10.0f;

	//����ó��
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	//��Ÿ�� ���� �÷���
	IsAttacking = false;

	//�޺�ó�� �߰�
	MaxComboCount = 4;
	AttackEndComboState();
}

// Called when the game starts or when spawned
void AT1Player::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AT1Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	if (CurrentCameraControlMode == ECameraControlMode::DIABLO )
	{
		SpringArm->RelativeRotation = FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);

		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
	}

}

// Called to bind functionality to input
void AT1Player::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	T1LOG_S(Warning);

	// ������Ʈ ���ÿ� ���õ� ��ǲ������ ������ �ɸ��� ��ǲ�� ���ε��ϴ� ���
	InputComponent->BindAxis(TEXT("UpDown"), this, &AT1Player::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &AT1Player::LeftRight);
	InputComponent->BindAxis(TEXT("Turn"), this, &AT1Player::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AT1Player::LookUp);

	// Ű�� �׼ǹ��ε�
	InputComponent->BindAction(TEXT("CameraViewChange"), EInputEvent::IE_Pressed, this, &AT1Player::CameraViewChange);	

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AT1Player::Jump);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AT1Player::Attack);
}


// �ݸ� ������ �ľ� �ϱ� ���� �������̵�	
void AT1Player::PostInitializeComponents()
{	
	Super::PostInitializeComponents();
	T1LOG_S(Warning);

	T1Anim = Cast<UT1AnimInstance>(GetMesh()->GetAnimInstance());
	T1CHECK(nullptr != T1Anim);

	/*
	  OnMontageEnded ��������Ʈ�� ��������Ʈ�� ȣȯ�Ǵ� �����ܿ��� �������� �Լ��� ������ �־
	  �ൿ�� ������ ��ϵ� ����Լ��鿡�� ��� �˷��ִ� ��ɵ� �����Ѵ�. �̷��� ��������Ʈ�� ��Ƽ�ɽ�Ʈ
	  ���� ����Ʈ(Multicast Delegate)��� �Ѵ�.

	  �ִ� �ν��Ͻ� ����� ����� OnMontageEnded�� ����ϴ� ��������Ʈ�� ������ �ڵ�� #include "Delegate.h" �� ����
	  �𸮾� �������� ��������Ʈ�� ������ �𸮾��� �����ϴ� ��ũ�θ� ���ؼ� ���ǵǸ�, �̷��� ���ǵ� ��������Ʈ ������
	  �ñ״�ó��� �Ѵ�.
	  ��ũ���̱⿡ ���ڸ������� �������� ����
	*/
	T1Anim->OnMontageEnded.AddDynamic(this, &AT1Player::OnAttackMontageEnded);

	/*
		UT1AnimInstance�� ���ǵ� ��������Ʈ�� ��ũ(��Ƽ���̸� ���� �Լ����)
		�ش���ٽ��� C++ ����
	*/

	T1Anim->OnNextAttackCheck.AddLambda([this]()->void
	{
		T1LOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			T1Anim->JumpToAttackMontageSection(CurrentComboIndex);
		}
	});
}

// �ݸ� ������ �ľ� �ϱ� ���� �������̵�	
void AT1Player::PossessedBy(AController* NewController)
{
	T1LOG_S(Warning);
	Super::PossessedBy(NewController);
}

void AT1Player::UpDown(float newAxisValue)
{
	//���ε����� ���� �̵�ó��
	if (CurrentCameraControlMode == ECameraControlMode::FOLLOW)
	{
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), newAxisValue);
	}
	else if (CurrentCameraControlMode == ECameraControlMode::DIABLO)
	{
		DirectionToMove.X = newAxisValue;
	}
	
}

void AT1Player::LeftRight(float newAxisValue)
{
	if (CurrentCameraControlMode == ECameraControlMode::FOLLOW)
	{
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), newAxisValue);
	}
	else if (CurrentCameraControlMode == ECameraControlMode::DIABLO)
	{
		DirectionToMove.Y = newAxisValue;
	}
	
}

void AT1Player::LookUp(float newAxisValue)
{
	if (CurrentCameraControlMode == ECameraControlMode::FOLLOW)
	{
		AddControllerPitchInput(newAxisValue);
	}
	
}

void AT1Player::Turn(float newAxisValue)
{
	if (CurrentCameraControlMode == ECameraControlMode::FOLLOW)
	{
		AddControllerYawInput(newAxisValue);
	}
}

void AT1Player::CameraViewChange()
{
	bool bFollow = CurrentCameraControlMode == ECameraControlMode::FOLLOW;
	GetController()->SetControlRotation(bFollow ? GetActorRotation() : SpringArm->RelativeRotation);
	SetCameraControlMode(bFollow ? ECameraControlMode::DIABLO : ECameraControlMode::FOLLOW);
}

void AT1Player::SetCameraControlMode(ECameraControlMode inMode)
{
	CurrentCameraControlMode = inMode;
	if (CurrentCameraControlMode == ECameraControlMode::FOLLOW)
	{
		/*SpringArm->TargetArmLength = 450.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);*/
		ArmLengthTo = 450.0f;
		// ��������(ī�޶�)�� ��Ʈ�ѷ� ���⿡ ��Ī
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = SpringArm->bInheritRoll = SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		// �ɸ���(��)�� ��Ʈ�ѷ��� Z��ȸ��(������ �������)
		bUseControllerRotationYaw = false;

		// ��Ʈ�ѷ��� �ٶ󺸴� �������� �̵��ϵ��� ����
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	}
	else if (CurrentCameraControlMode == ECameraControlMode::DIABLO)
	{
		ArmLengthTo = 900.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = SpringArm->bInheritRoll = SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;

		// ������ ���⶧���� �ּ�
		//bUseControllerRotationYaw = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	}
}

void AT1Player::Attack()
{
	T1LOG_S(Warning);

	if (IsAttacking)
	{
		T1CHECK(FMath::IsWithinInclusive<int32>(CurrentComboIndex, 1, MaxComboCount));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		T1CHECK(CurrentComboIndex == 0);
		AttackStartComboState();
		T1Anim->PlayAttackMontage();
		T1Anim->JumpToAttackMontageSection(CurrentComboIndex);
		IsAttacking = true;

	}	
}

void AT1Player::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	T1CHECK(FMath::IsWithinInclusive<int32>(CurrentComboIndex, 0, MaxComboCount - 1));
	CurrentComboIndex = FMath::Clamp<int32>(CurrentComboIndex + 1, 1, MaxComboCount);
}

void AT1Player::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentComboIndex = 0;
}

void AT1Player::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruped)
{
	T1CHECK(IsAttacking);
	IsAttacking = false;
	AttackEndComboState();
}

