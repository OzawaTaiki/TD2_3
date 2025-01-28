#include "TitleScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>
#include <Physics/Collision/CollisionManager.h>

std::unique_ptr<BaseScene> TitleScene::Create()
{
	return std::make_unique<TitleScene>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_.y = 45;
    SceneCamera_.translate_.z = -1.5f;

    SceneCamera_.rotate_.x = std::numbers::pi_v<float> / 2.0f;

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

    const size_t enemyNum = 20;

    for (size_t i = 0; i < enemyNum; i++)
    {
        auto& enemy = enemies_.emplace_back(std::make_unique<TitleEnemy>());
        enemy->Initialize(&SceneCamera_);
        enemy->SetTranslate(Vector3{ static_cast<float>(-20.0f + i * 2),0,0 });
        enemy->SetRangeOfMovement({ 20.0f,0.0f,0.0f }, { -20.0f,0.0f,0.0f });
        enemy->SetMoveSpeed(-0.1f);
    };

    enemyManager_ = std::make_unique<EnemyManager>();

    player_ = std::make_unique<Player>();
    player_->Initialize(&SceneCamera_);

    BG_.Initialize("TItleBG");

    titleUI_ = std::make_unique<TItleUI>();
    titleUI_->Initialize();

    DirectionalLight DLight{};
    DLight.direction = Vector3(-0.909f, -0.417f, 0.0f).Normalize();
    DLight.intensity = 1.0f;

    lg.Initialize();
    lg.SetDirectionalLight(DLight);

    titleLogo_ = std::make_unique<ObjectModel>();
    titleLogo_->Initialize("TitleLogo/title.obj", "title");
    float pi = std::numbers::pi_v<float>;

    titleLogo_->translate_ = { 0,-1,4 };
    titleLogo_->scale_ = { 4,4,1 };
    titleLogo_->rotate_ = { pi / 2.0f,0,pi };


    LightingSystem::GetInstance()->SetLightGroup(&lg);
}

void TitleScene::Update()
{
    CollisionManager::GetInstance()->ResetColliderList();

    //if(Input::GetInstance()->IsKeyTriggered(DIK_TAB) ||
    //   Input::GetInstance()->IsPadTriggered(PadButton::iPad_A))
    //{
    //    SceneManager::GetInstance()->ReserveScene("Game");
    //}

    titleLogo_->Update();
    player_->Update();

#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN))
    {
        enemyMove_ = !enemyMove_;
    }

    BG_.Update();
    titleUI_->Update();

    //if (enemyMove_)
#endif // _DEBUG

    for (auto& enemy : enemies_)
    {
        enemy->Update();
    }

    if(enemyManager_->AttractEnemy(enemies_))
    {
        SceneManager::ReserveScene("Game");
    }


    SceneCamera_.Update();
    SceneCamera_.UpdateMatrix();

    player_->SetHp(100.0f);

    CollisionManager::GetInstance()->CheckAllCollision();
}

void TitleScene::Draw()
{
    Sprite::PreDraw();
    BG_.Draw();

    titleLogo_->Draw(&SceneCamera_, { 1,1,1,1 });

    player_->Draw({ 0,0,0,1 });

    for (auto& enemy : enemies_)
    {
        enemy->Draw({ 0,0,0,1 });
    }

    Sprite::PreDraw();
    titleUI_->Draw();

    lineDrawer_->Draw();
}
#ifdef _DEBUG
void TitleScene::ImGui()
{
}
#endif // DEBUG
