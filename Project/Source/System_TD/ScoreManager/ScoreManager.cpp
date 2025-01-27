#include "ScoreManager.h"
#include "imgui.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return &instance;
}

void ScoreManager::Initialize()
{



	InitJsonBinder();
}

void ScoreManager::Update()
{
	saveMaxScore_ = currentScore_;




	jsonBinder_->Save();
#ifdef _DEBUG
	ImGui();
#endif // _DEBUG
}

void ScoreManager::AddScore(int add)
{
	currentScore_ += add;
}

void ScoreManager::ResetScore()
{
	currentScore_ = 0;
}

void ScoreManager::DrawScore()
{

}

void ScoreManager::ImGui()
{
	ImGui::Begin("Score Info");

	ImGui::DragInt("Current Score", &currentScore_);



	ImGui::End();
}

void ScoreManager::InitJsonBinder()
{
	jsonBinder_ = std::make_unique<JsonBinder>("ScoreData", "Resources/Data/Score/");

	jsonBinder_->RegisterVariable("CurrentScore", &currentScore_);
	jsonBinder_->RegisterVariable("MaxScore", &saveMaxScore_);
}
