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

    titleUI_ = std::make_unique<TItleUI>();
    titleUI_->Initialize();
}

void TitleScene::Update()
{
    CollisionManager::GetInstance()->ResetColliderList();

    //if(Input::GetInstance()->IsKeyTriggered(DIK_TAB) ||
    //   Input::GetInstance()->IsPadTriggered(PadButton::iPad_A))
    //{
    //    SceneManager::GetInstance()->ReserveScene("Game");
    //}

    player_->Update();

#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN))
    {
        enemyMove_ = !enemyMove_;
    }

    if (enemyMove_)
#endif // _DEBUG

    for (auto& enemy : enemies_)
    {
        enemy->Update();
    }

    if(enemyManager_->AttractEnemy(enemies_))
    {
        SceneManager::ReserveScene("Game");
    }

    titleUI_->Update();

    SceneCamera_.Update();
    SceneCamera_.UpdateMatrix();

    CollisionManager::GetInstance()->CheckAllCollision();
}

void TitleScene::Draw()
{
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
