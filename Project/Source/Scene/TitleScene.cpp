#include "TitleScene.h"
#include "eScene/SceneManager.h"
std::unique_ptr<BaseScene> TitleScene::Create()
{
	return std::unique_ptr<TitleScene>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	// シーン変更処理
	//SceneManager::GetInstance()->ReserveScene("Game");
}

void TitleScene::Draw()
{
}
#ifdef _DEBUG
#include <imgui.h>
void TitleScene::ImGui()
{
}
#endif // _DEBUG