#pragma once

// 로비 UI에서 C++쪽으로 타이밍등 이벤트 전달용
UENUM(BluePrintType)
enum class EGS_LOBBY_Enum : uint8
{
	GS_LOBBY_START_INTRO	UMETA(DisplayName = "LOBBY_START_INTRO"),
	GS_LOBBY_END_INTRO		UMETA(DisplayName = "LOBBY_END_INTRO"),
	GS_LOBBY_BACKTO_SERVER_SELECT UMETA(DisplayName = "LOBBY_BACKTO_SERVER_SELECT"),
	GS_LOBBY_ENTER_INGAME UMETA(DisplayName = "LOBBY_ENTER_INGAME"),
	GS_LOBBY_INGAME_LOAD_COMPLETE UMETA(DisplayName = "LOBBY_INGAME_LOAD_COMPLETE")
};