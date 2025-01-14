#include "GameScene.h"

#include <Framework/eScene/BaseScene.h>
#include <Rendering/Model/ModelManager.h>
#include <Rendering/Light/LightingSystem.h>
#include <Rendering/LineDrawer/LineDrawer.h>


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

    collisionManager_ = CollisionManager::GetInstance();

    player_ = std::make_unique<Player>();
    player_->Initialize(&SceneCamera_);
    followCamera_.SetTarget(player_->GetWorldTransform());

    enemyManager_ = std::make_unique<EnemyManager>();
    enemyManager_->Initialize(&SceneCamera_);

    lightGroup_.Initialize();
    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);

    LineDrawer::GetInstance()->SetCameraPtr(&SceneCamera_);
}

void GameScene::Update()
{
    /*===============================================================//
                         　　    当たり判定
    //===============================================================*/
    collisionManager_->ResetColliderList();

#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
        Input::GetInstance()->IsKeyPressed(DIK_LSHIFT)) {
        enableDebugCamera_ = !enableDebugCamera_;
    }
#endif // _DEBUG

    
    player_->Update();
    enemyManager_->Update();

    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        SceneCamera_.TransferData();
    }
    else
    {
        followCamera_.Update();
        SceneCamera_.matView_ = followCamera_.matView_;
        SceneCamera_.translate_ = followCamera_.translate_;
        SceneCamera_.rotate_ = followCamera_.rotate_;
        SceneCamera_.Update();
        SceneCamera_.UpdateMatrix();
    }

    collisionManager_->CheckAllCollision();
}

void GameScene::Draw()
{
    ModelManager::GetInstance()->PreDrawForObjectModel();

    player_->Draw({ 1,1,1,1 });
    enemyManager_->Draw({ 1,1,1,1 });

    LineDrawer::GetInstance()->Draw();
}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{

}
#endif // _DEBUG