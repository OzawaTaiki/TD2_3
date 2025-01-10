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
    followCamera_.Initialize();

    player_ = std::make_unique<Player>();
    player_->Initialize(&SceneCamera_);
    followCamera_.SetTarget(player_->GetWorldTransform());
}

void GameScene::Update()
{
#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
        Input::GetInstance()->IsKeyPressed(DIK_RSHIFT))
        enableDebugCamera_ = !enableDebugCamera_;
#endif // _DEBUG


    player_->Update();

    if (enableDebugCamera_)
    {
        // デバッグカメラの更新
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
    }
    else
    {
        // 追従カメラの更新
        followCamera_.Update();

        // フォローカメラのビュー行列をシーンカメラに適用
        SceneCamera_.matView_ = followCamera_.matView_;

        // シーンカメラの行列を更新
        SceneCamera_.UpdateMatrix();
    }

    // カメラ情報をGPUに転送
    SceneCamera_.TransferData();

}

void GameScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();

    player_->Draw({ 1,1,1,1 });

}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{

}
#endif // _DEBUG