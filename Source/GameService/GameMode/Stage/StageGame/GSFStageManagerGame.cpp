#include "GSFStageManagerGame.h"


//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
GSFStageGameBase* GSFStageGameAllocator::Alloc(GSFStageMode::Game inMode)
{
	return NULL;
}
//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
void GSStageManagerGame::RemoveAll()
{
	Super::RemoveAll();
}
void GSStageManagerGame::InitState()
{
	Super::InitState();
}
