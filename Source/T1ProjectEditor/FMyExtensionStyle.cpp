#include "FMyExtensionStyle.h"

#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr< FSlateStyleSet > FMyExtensionStyle::StyleInstance = nullptr;

void FMyExtensionStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMyExtensionStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMyExtensionStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MyExtensionStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FMyExtensionStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MyExtensionStyle"));
	Style->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));

	// Menu Icons
	Style->Set("MyExtensions.DataTool", new IMAGE_BRUSH("Icons/icon_file_switch_16px", Icon16x16));	

	// Toolbar Icons
	Style->Set("MyToolbarIcon.DataTool", new IMAGE_BRUSH("Icons/icon_Landscape_Tool_Erosion_40x", Icon40x40));	

	return Style;
}

#undef IMAGE_BRUSH

void FMyExtensionStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FMyExtensionStyle::Get()
{
	return *StyleInstance;
}