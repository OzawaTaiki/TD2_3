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

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);

}

void TitleScene::Update()
{
    if(Input::GetInstance()->IsKeyTriggered(DIK_TAB))
    {
        SceneManager::GetInstance()->ReserveScene("Game");
    }


    lineDrawer_->DrawCircle({ 0,0,0 }, 1);
    SceneCamera_.Update();
    SceneCamera_.UpdateMatrix();
}

void TitleScene::Draw()
{
    lineDrawer_->Draw();
}
#ifdef _DEBUG
void TitleScene::ImGui()
{
}
#endif // DEBUG
