#include "ComboManager.h"
#include <ResourceManagement/TextureManager/TextureManager.h>
#include"imgui.h"
ComboManager* ComboManager::GetInstance()
{
    static ComboManager instance;
    return &instance;
}

void ComboManager::Initialize()
{
	//uint32_t th = TextureManager::GetInstance()->Load("");
	//sprite_ = std::make_unique<Sprite>();
	//sprite_->Create(th);
	//sprite_->Initialize();

	//sprite_->translate_ = { 0,0 };
	//sprite_->SetColor({ 0,0,0,1 });

	// Json初期化
	InitJsonBinder();
}

void ComboManager::Update()
{



	jsonBinder_->Save();
#ifdef _DEBUG
	ImGui();
#endif // _DEBUG
}

void ComboManager::Draw()
{
	Sprite::PreDraw();
	sprite_->Draw();
}

void ComboManager::EndGame()
{
}

void ComboManager::UpdateTopCombos()
{
}

void ComboManager::AddCombo(int count)
{
	currentCombo_ += count;

}

void ComboManager::ImGui()
{
	ImGui::Begin("ComboManager");

	ImGui::DragInt("Current Combo", &currentCombo_);
	ImGui::DragInt("Max Combo", &saveMaxCombo_);


	
	// 保存
	if (ImGui::Button("Save")) {
		jsonBinder_->Save();
	}
	ImGui::End();
}

void ComboManager::InitJsonBinder()
{
	jsonBinder_ = std::make_unique<JsonBinder>("ComboData", "Resources/Data/Combo/");

	jsonBinder_->RegisterVariable("CurrentCombo", &currentCombo_);
	jsonBinder_->RegisterVariable("MaxCombo", &saveMaxCombo_);


}


