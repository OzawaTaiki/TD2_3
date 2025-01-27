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
	currentCombo_ = 0;
	InitJsonBinder();


	lastComboTime_ = std::chrono::steady_clock::now();
}

void ComboManager::Update()
{
	// 現在時刻を取得
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastComboTime_).count();

	// 一定時間経過したらコンボリセット
	if (elapsed >= comboResetTime_) {
		currentCombo_ = 0;
	}

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
	saveMaxCombo_ = currentCombo_;

	UpdateTopCombos();

	jsonBinder_->Save();

	JsonHub::GetInstance()->LoadFile("Resources/Data/Combo/ComboData.json");
}

void ComboManager::UpdateTopCombos()
{
	// 最終的なスコアを追加
	topCombos_.push_back(saveMaxCombo_);
	std::sort(topCombos_.rbegin(), topCombos_.rend());

	// 重複するスコアを削除
	auto last = std::unique(topCombos_.begin(), topCombos_.end());
	topCombos_.erase(last, topCombos_.end());

	// トップ3スコアのみ保持
	if (topCombos_.size() > 3)
	{
		topCombos_.resize(3);
	}
}

void ComboManager::AddCombo(int count)
{
	currentCombo_ += count;
	lastComboTime_ = std::chrono::steady_clock::now();
}

void ComboManager::ImGui()
{
	ImGui::Begin("ComboManager");

	ImGui::DragInt("Current Combo", &currentCombo_);
	ImGui::DragInt("Max Combo", &saveMaxCombo_);


	for (size_t i = 0; i < topCombos_.size(); ++i)
	{
		ImGui::Text("%zu: %d", i + 1, topCombos_[i]);
	}


	
	// 保存
	if (ImGui::Button("Save")) {
		jsonBinder_->Save();
	}
	ImGui::End();
}

void ComboManager::InitJsonBinder()
{
	jsonBinder_ = std::make_unique<JsonBinder>("ComboData", "Resources/Data/Combo/");

	jsonBinder_->RegisterVariable("MaxCombo", &saveMaxCombo_);
	jsonBinder_->RegisterVariable("Top3_Scores", &topCombos_);

}


