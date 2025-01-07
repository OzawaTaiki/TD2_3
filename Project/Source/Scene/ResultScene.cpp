#include "ResultScene.h"

std::unique_ptr<BaseScene> ResultScene::Create()
{
	return std::make_unique<ResultScene>();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
}
#ifdef _DEBUG
void ResultScene::ImGui()
{
}
#endif // DEBUG
