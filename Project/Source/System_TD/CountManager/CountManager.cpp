#include "CountManager.h"
#include <imgui.h>
CountManager* CountManager::GetInstance()
{
	static CountManager instance;
	return &instance;
}
void CountManager::Initialize()
{
	enemyCount_ = 0;
}

void CountManager::Draw()
{
}

void CountManager::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("CountManager");
	ImGui::Text("EnemyCount : %d", enemyCount_);

	ImGui::End();

#endif // _DEBUG

}

void CountManager::EnemyCount(uint32_t count)
{
	enemyCount_ += count;
}
