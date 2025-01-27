#include "ScoreManager.h"
#include "imgui.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return &instance;
}

void ScoreManager::Initialize()
{

	currentScore_ = 0;

	InitJsonBinder();
}

void ScoreManager::Update()
{


	//UpdateTopScores();
	
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
void ScoreManager::EndGame()
{
	
	saveMaxScore_ = currentScore_;
	
	UpdateTopScores();

	jsonBinder_->Save();

	JsonHub::GetInstance()->LoadFile("Resources/Data/Score/ScoreData.json");
}


void ScoreManager::ImGui()
{
	ImGui::Begin("Score Info");

	ImGui::DragInt("Current Score", &currentScore_);

	for (size_t i = 0; i < topScores_.size(); ++i)
	{
		ImGui::Text("%zu: %d", i + 1, topScores_[i]);
	}


	ImGui::End();
}

void ScoreManager::InitJsonBinder()
{
	jsonBinder_ = std::make_unique<JsonBinder>("ScoreData", "Resources/Data/Score/");

	//jsonBinder_->RegisterVariable("CurrentScore", &currentScore_);
	jsonBinder_->RegisterVariable("MaxScore", &saveMaxScore_);
	jsonBinder_->RegisterVariable("Top3_Scores", &topScores_);
}

void ScoreManager::UpdateTopScores()
{
	// 最終的なスコアを追加
	topScores_.push_back(saveMaxScore_);
	std::sort(topScores_.rbegin(), topScores_.rend()); 

	// 重複するスコアを削除
	auto last = std::unique(topScores_.begin(), topScores_.end());
	topScores_.erase(last, topScores_.end());

	// トップ3スコアのみ保持
	if (topScores_.size() > 3)
	{
		topScores_.resize(3);
	}
}
