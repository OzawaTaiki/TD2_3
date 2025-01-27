#include "ResultScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>
#include "../System_TD/ScoreManager/ScoreManager.h"
#include "../System_TD/ComboManager/ComboManager.h"
#include "../System_TD/CountManager/CountManager.h"
#include <ResourceManagement/TextureManager/TextureManager.h>

const std::string defaulFilPath = "Resources./Textures./";

std::unique_ptr<BaseScene> ResultScene::Create()
{
	return std::make_unique<ResultScene>();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	uint32_t th[ms_] = { TextureManager::GetInstance()->Load("backGround.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("frame.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("result.png", defaulFilPath) };
	
	resultSprite_[0] = std::make_unique<Sprite>();
	resultSprite_[0].reset(Sprite::Create(th[0]));
	resultSprite_[0]->Initialize();
	resultSprite_[0]->translate_ = {640,360};

	resultSprite_[1] = std::make_unique<Sprite>();
	resultSprite_[1].reset(Sprite::Create(th[1]));
	resultSprite_[1]->Initialize();
	resultSprite_[1]->translate_ = { 640,360 };

	resultSprite_[2] = std::make_unique<Sprite>();
	resultSprite_[2].reset(Sprite::Create(th[2]));
	resultSprite_[2]->Initialize();
	resultSprite_[2]->SetAnchor({ 0.5f,0.5f });
	resultSprite_[2]->translate_ = { 640,360 };
	

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
	Sprite::PreDraw();
	for (uint32_t i = 0; i < ms_; ++i)
	{
		resultSprite_[i]->Draw();
	}
}
#ifdef _DEBUG
void ResultScene::ImGui()
{
}
#endif // DEBUG
