#include "GameScene.h"

#include <Framework/eScene/BaseScene.h>

std::unique_ptr<BaseScene>GameScene::Create()
{
    return std::make_unique<GameScene>();
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{

}
#endif // _DEBUG