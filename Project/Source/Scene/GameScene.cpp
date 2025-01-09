#include "GameScene.h"

#include <Framework/eScene/BaseScene.h>
#include <Rendering/Model/ModelManager.h>

#include <DirectXMath.h>

std::unique_ptr<BaseScene>GameScene::Create()
{
    return std::make_unique<GameScene>();
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,100,0 };
    SceneCamera_.rotate_ = { DirectX::XMConvertToRadians(90.0f),0,0 };
    SceneCamera_.UpdateMatrix();
    debugCamera_.Initialize();

    player_ = std::make_unique<Player>();
    player_->Initialize();
}

void GameScene::Update()
{

    player_->Update();

    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        SceneCamera_.TransferData();
    }
    else
    {
        SceneCamera_.Update();
        SceneCamera_.UpdateMatrix();
    }


}

void GameScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();

    player_->Draw(SceneCamera_, { 1,1,1,1 });

}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{

}
#endif // _DEBUG