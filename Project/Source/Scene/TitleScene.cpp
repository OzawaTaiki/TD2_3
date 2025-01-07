#include "TitleScene.h"

#include "TitleScene.h"

std::unique_ptr<BaseScene> TitleScene::Create()
{
	return std::make_unique<TitleScene>();
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
#ifdef DEBUG
void TitleScene::ImGui()
{
}
#endif // DEBUG
