#include "ScoreManager.h"
#include "imgui.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return &instance;
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


	ImGui();
}

void ScoreManager::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Score Info");

	ImGui::DragInt("Current Score", &currentScore_);

	ImGui::End();

#endif // _DEBUG

}
