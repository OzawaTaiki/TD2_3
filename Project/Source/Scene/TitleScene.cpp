#include "TitleScene.h"

std::unique_ptr<BaseScene> TitleScene::Create()
{
	return std::unique_ptr<BaseScene>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	
}

void TitleScene::Draw()
{
}
#ifdef _DEBUG
#include <imgui.h>
void TitleScene::ImGui()
{
}
#endif // _DEBUG