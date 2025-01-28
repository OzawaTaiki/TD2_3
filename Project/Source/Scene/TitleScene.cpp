#include "TitleScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>

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
    SceneCamera_.translate_.y = 30;
    SceneCamera_.translate_.z = 0;

    SceneCamera_.rotate_.x = std::numbers::pi_v<float> / 2.0f;

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

    for (size_t i = 0; i < enemies_.size(); i++)
    {
        enemies_[i] = std::make_unique<TitleEnemy>();
        enemies_[i]->Initialize(&SceneCamera_);
        enemies_[i]->SetTranslate(Vector3{ static_cast<float>(-20.0f+i * 2),0,0 });
        enemies_[i]->SetRangeOfMovement({ 20.0f,0.0f,0.0f }, { -20.0f,0.0f,0.0f });
        enemies_[i]->SetMoveSpeed(-0.1f);
    };


}

void TitleScene::Update()
{
    if(Input::GetInstance()->IsKeyTriggered(DIK_TAB))
    {
        SceneManager::GetInstance()->ReserveScene("Game");
    }

#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN))
    {
        enemyMove_ = !enemyMove_;
    }

    if(enemyMove_)
#endif // _DEBUG

    for (size_t i = 0; i < enemies_.size(); i++)
    {
        enemies_[i]->Update();
    }


    SceneCamera_.Update();
    SceneCamera_.UpdateMatrix();
}

void TitleScene::Draw()
{
    for (size_t i = 0; i < enemies_.size(); i++)
    {
        enemies_[i]->Draw({ 1,1,1,1 });
    }

    lineDrawer_->Draw();
}
#ifdef _DEBUG
void TitleScene::ImGui()
{
}
#endif // DEBUG
