#include "TitleScene.h"

#include "TitleScene.h"

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
