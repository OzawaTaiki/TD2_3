#include "GameScene.h"

#include "eScene/SceneManager.h"

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
    // シーン変更処理
    //SceneManager::GetInstance()->ReserveScene("Result");
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