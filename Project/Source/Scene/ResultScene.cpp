#include "ResultScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>

std::unique_ptr<BaseScene> ResultScene::Create()
{
	return std::make_unique<ResultScene>();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

void ResultScene::Update()
{
    if (Input::GetInstance()->IsKeyTriggered(DIK_TAB))
    {
        SceneManager::GetInstance()->ReserveScene("Title");
    }
}

void ResultScene::Draw()
{
}
#ifdef _DEBUG
void ResultScene::ImGui()
{
}
#endif // DEBUG
