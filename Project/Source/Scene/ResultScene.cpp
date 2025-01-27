#include "ResultScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>
#include "../System_TD/ScoreManager/ScoreManager.h"
#include "../System_TD/ComboManager/ComboManager.h"
#include "../System_TD/CountManager/CountManager.h"

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
    if (Input::GetInstance()->IsKeyTriggered(DIK_TAB) || 
        Input::GetInstance()->IsPadTriggered(PadButton::iPad_A))
    {
        SceneManager::GetInstance()->ReserveScene("Title");
    }

	ScoreManager::GetInstance()->Update();
	ComboManager::GetInstance()->Update();
	CountManager::GetInstance()->ImGui();
}

void ResultScene::Draw()
{
}
#ifdef _DEBUG
void ResultScene::ImGui()
{
}
#endif // DEBUG
