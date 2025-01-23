#include "GameScene.h"

#include <Framework/eScene/SceneManager.h>
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
    if (loadThread_ && loadThread_->joinable())
    {
        loadThread_->join();
    }

    delete loadScene_;
    delete area_;
}

void GameScene::Initialize()
{
    loadScene_ = new LoadScene();
    loadScene_->Initialize();
    Loading_ = true;

    loadThread_ = std::make_unique <std::thread>(&GameScene::Load, this);
    //loadThread_->detach();
}

void GameScene::Update()
{
    if (Loading_)
    {
        loadScene_->Update();
        return;
    }

    /*===============================================================//
                         　　    当たり判定
    //===============================================================*/
    collisionManager_->ResetColliderList();

#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
        Input::GetInstance()->IsKeyPressed(DIK_LSHIFT)) {
        enableDebugCamera_ = !enableDebugCamera_;
    }

    lightGroup_.DrawDebugWindow();
    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);
#endif // _DEBUG

    if(!player_->IsAlive())
    {
        SceneManager::GetInstance()->ReserveScene("Result");
    }


    player_->Update();
    enemyManager_->Update();
    area_->Update(player_.get());

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
    if (Loading_)
    {
        loadScene_->Draw();
        return;
    }

    ModelManager::GetInstance()->PreDrawForObjectModel();
    area_->Draw(&SceneCamera_);

    player_->Draw({ 1,1,1,1 });
    enemyManager_->Draw({ 1,1,1,1 });

    LineDrawer::GetInstance()->Draw();
}

void GameScene::Load()
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
    enemyManager_->SetPlayer(player_.get());
    enemyManager_->Initialize(&SceneCamera_);



    lightGroup_.Initialize();
    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);

    LineDrawer::GetInstance()->SetCameraPtr(&SceneCamera_);


    area_ = new Area();
    area_->Initialize(50);

    DirectionalLight DLight{};
    DLight.direction = Vector3(-0.909f, -0.417f, 0.0f).Normalize();
    DLight.intensity = 0.2f;

    PointLight PLight{};
    PLight.position = Vector3(0, 1.4f, 0);
    PLight.intensity = 2.0f;
    PLight.radius = 20.0f;
    PLight.decay = 2.0f;

    lightGroup_.SetDirectionalLight(DLight);
    lightGroup_.AddPointLight(PLight, "", player_->GetWorldPositionRef());
    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);

    //Model::CreateFromObj("bunny.obj");

    Loading_ = false;

}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{

}
#endif // _DEBUG