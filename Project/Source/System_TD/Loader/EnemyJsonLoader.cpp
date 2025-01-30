#define _CRT_SECURE_NO_WARNINGS
#include "EnemyJsonLoader.h"
#include <json.hpp>
#include <fstream>
#include <cassert>
#include "../../Objects/Entity/Player/Player.h"
using json = nlohmann::json;

/// <summary>
/// 敵発生データの読み込み(JSON版)
/// </summary>
void EnemyJsonLoader::LoadEnemyPopData()
{
	std::ifstream file("Resources/Data/EnemySpawn/EnemySpawnData.json");

	if (!file.is_open()) {
		waves_.clear();
		Wave defaultWave;
		defaultWave.waveNumber = 1;
		defaultWave.groups.push_back({ 5, "Default", 1.0f, Vector3(0,0,0), Vector3(0,0,0), Vector3(1,0,0), 1.0f, 0 });
		waves_.push_back(defaultWave);
		SaveEnemyPopData();
		return;
	}

	json jsonData;
	file >> jsonData;
	file.close();

	waves_ = jsonData.get<std::vector<Wave>>();
}

/// <summary>
/// 敵発生データの保存
/// </summary>
void EnemyJsonLoader::SaveEnemyPopData() const
{
	json jsonData = waves_;
	std::ofstream file("Resources/Data/EnemySpawn/EnemySpawnData.json");
	if (file.is_open()) {
		file << jsonData.dump(4);
		file.close();
	}
}

/// <summary>
/// 敵発生コマンドの更新
/// </summary>
void EnemyJsonLoader::UpdateEnemyPopCommands()
{
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) isWait_ = false;
		return;
	}

	
	UpdateEnemyPositions();

	UpdateWave();
}
void EnemyJsonLoader::UpdateWave()
{
	if (currentWaveIndex_ >= waves_.size()) return;

	Wave& currentWave = waves_[currentWaveIndex_];

	// グループの処理が終わった場合
	if (currentGroupIndex_ >= currentWave.groups.size()) {
		// ウェーブの待機時間を適用
		if (currentWave.waitTime > 0) {
			isWait_ = true;
			waitTimer_ = static_cast<int>(currentWave.waitTime * 60); // 秒 → フレーム変換
		}
		// 次のウェーブへ移行
		currentWaveIndex_++;
		currentGroupIndex_ = 0;
		return;
	}

	// グループの処理を実行
	UpdateGroup();
}

void EnemyJsonLoader::UpdateGroup()
{
	Wave& currentWave = waves_[currentWaveIndex_];
	Group& currentGroup = currentWave.groups[currentGroupIndex_];

	for (uint32_t i = 0; i < currentGroup.numEnemies; ++i) {
		Vector3 position = currentGroup.spawnPoint + currentGroup.direction * currentGroup.offset * i;
		float speed = currentGroup.speed;
		Vector3 goal = currentGroup.goal;
		std::string moveType = currentGroup.moveType;

		// MoveType が "Target" の場合、プレイヤーの位置を取得
		if (currentGroup.moveType == "Target" && player_) {
			goal = player_->GetWorldPosition();  // プレイヤーの現在位置をゴールにする
		}

		if (!isUpdate_) {
			speed = 0.0f; // 速度を0にして移動を停止
		}

		if (spawnCallback_) {
			spawnCallback_(position, speed, goal, moveType);
		}
	}

	// グループの待機時間を適用
	if (currentGroup.waitTime > 0) {
		isWait_ = true;
		waitTimer_ = static_cast<int>(currentGroup.waitTime * 60); // 秒 → フレーム変換
	}

	// 次のグループへ
	currentGroupIndex_++;
}

void EnemyJsonLoader::IUpdateWave(int waveIndex)
{
	if (waveIndex < 0 || waveIndex >= waves_.size()) return;

	Wave& wave = waves_[waveIndex];

	// ウェーブの待機時間を適用
	if (wave.waitTime > 0) {
		isWait_ = true;
		waitTimer_ = static_cast<int>(wave.waitTime * 60); // 秒 → フレーム変換
	}

	// ウェーブ内のすべてのグループを更新
	for (size_t groupIndex = 0; groupIndex < wave.groups.size(); ++groupIndex) {
		IUpdateGroup(waveIndex, static_cast<int>(groupIndex));
	}
}

void EnemyJsonLoader::IUpdateGroup(int waveIndex, int groupIndex)
{
	if (waveIndex < 0 || waveIndex >= waves_.size()) return;
	Wave& wave = waves_[waveIndex];

	if (groupIndex < 0 || groupIndex >= wave.groups.size()) return;
	Group& group = wave.groups[groupIndex];

	for (uint32_t i = 0; i < group.numEnemies; ++i) {
		Vector3 position = group.spawnPoint + group.direction * group.offset * i;
		float speed = group.speed;
		Vector3 goal = group.goal;
		std::string moveType = group.moveType;

		// MoveType が "Target" の場合、プレイヤーの位置を取得
		if (group.moveType == "Target" && player_) {
			goal = player_->GetWorldPosition();
		}

		if (!isUpdate_) {
			speed = 0.0f; // 速度を0にして移動を停止
		}

		if (spawnCallback_) {
			spawnCallback_(position, speed, goal, moveType);
		}
	}

	// グループの待機時間を適用
	if (group.waitTime > 0) {
		isWait_ = true;
		waitTimer_ = static_cast<int>(group.waitTime * 60);
	}
}

void EnemyJsonLoader::UpdateEnemyPositions()
{
	if (!player_) return;

	for (auto& wave : waves_) {
		for (auto& group : wave.groups) {
			if (group.moveType == "Target") {
				group.goal = player_->GetWorldPosition();
			}
		}
	}
}


/// <summary>
/// ImGui で敵データを編集し、JSON に保存
/// </summary>
void EnemyJsonLoader::ShowImGui()
{
	static int pendingWaveDeletion = -1;
	static int pendingGroupDeletionWave = -1;
	static int pendingGroupDeletion = -1;
	static bool showDeleteWarning = false;
	static bool deleteWave = false;
	static bool deleteGroup = false;

	if (ImGui::Begin("Enemy Spawn Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Wave Management");
		ImGui::Separator();

		// ウェーブの追加
		if (ImGui::Button("Add New Wave", ImVec2(200, 30))) {
			Wave newWave;
			newWave.waveNumber = waves_.empty() ? 1 : waves_.back().waveNumber + 1;
			newWave.waitTime = 1.0f; // 初期ウェーブタイマーを 1 秒に設定
			waves_.push_back(newWave);
			SaveEnemyPopData();
		}

		// ウェーブの追加
		if (ImGui::Button("Update All Wave", ImVec2(200, 30))) {

			UpdateEnemyPopCommands();
		}

		for (size_t i = 0; i < waves_.size(); ++i) {
			Wave& wave = waves_[i];

			// Push unique ID to prevent ID conflicts
			ImGui::PushID(i);

			if (ImGui::CollapsingHeader(("Wave " + std::to_string(wave.waveNumber)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Indent();

				// ウェーブの待機時間（タイマー設定）
				ImGui::DragFloat(("Wave Timer (seconds)##" + std::to_string(i)).c_str(), &wave.waitTime, 0.1f, 0.0f, 30.0f, "%.2f sec");

				// 指定ウェーブの更新ボタン
				if (ImGui::Button(("Update Wave " + std::to_string(wave.waveNumber)).c_str(), ImVec2(140, 20))) {
					IUpdateWave(i);
				}

				ImGui::SameLine();
				if (ImGui::Button(("Delete Wave " + std::to_string(wave.waveNumber)).c_str(), ImVec2(140, 20))) {
					pendingWaveDeletion = static_cast<int>(i);
					showDeleteWarning = true;
					deleteWave = true;
				}

				// グループ管理
				ImGui::Text("Group Management");
				ImGui::Separator();

				if (ImGui::Button(("Add Group to Wave##" + std::to_string(i)).c_str(), ImVec2(140, 20))) {
					Group newGroup;
					newGroup.numEnemies = 5;
					newGroup.moveType = "Default";
					newGroup.speed = 1.0f;
					newGroup.goal = Vector3(0, 0, 0);
					newGroup.spawnPoint = Vector3(0, 0, 0);
					newGroup.direction = Vector3(1, 0, 0);
					newGroup.offset = 2.0f;
					newGroup.waitTime = 1;
					wave.groups.push_back(newGroup);
					SaveEnemyPopData();
				}

				for (size_t j = 0; j < wave.groups.size(); ++j) {
					Group& group = wave.groups[j];

					// Push unique ID to prevent conflicts
					ImGui::PushID(j);

					if (ImGui::TreeNode(("Group " + std::to_string(j)).c_str())) {
						ImGui::PushItemWidth(200);
						ImGui::DragInt(("Num Enemies##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &group.numEnemies, 1, 1, 100);
						ImGui::DragFloat(("Speed##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &group.speed, 0.1f, 0.1f, 10.0f);
						ImGui::DragFloat3(("Goal##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &group.goal.x, 0.1f);
						ImGui::DragFloat3(("Spawn Point##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &group.spawnPoint.x, 0.1f);
						ImGui::DragFloat3(("Direction##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &group.direction.x, 0.1f);
						ImGui::DragFloat(("Offset##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &group.offset, 0.1f, 0.0f, 10.0f);
						ImGui::DragFloat("Wait Time (seconds)", &group.waitTime, 0.5f, 1.0f, 10.0f, "%.2f sec");
						ImGui::PopItemWidth();

						// MoveType の選択式コンボボックス
						const char* moveTypeOptions[] = { "Default", "Target" };
						int currentMoveType = (group.moveType == "Target") ? 1 : 0;

						if (ImGui::Combo(("Move Type " + std::to_string(i) + "_" + std::to_string(j)).c_str(), &currentMoveType, moveTypeOptions, IM_ARRAYSIZE(moveTypeOptions))) {
							group.moveType = moveTypeOptions[currentMoveType];
						}

						ImGui::Spacing();
						// 指定グループの更新ボタン
						if (ImGui::Button(("Update Group " + std::to_string(j)).c_str(), ImVec2(140, 20))) {
							IUpdateGroup(i, j);
						}


						ImGui::Spacing();
						if (ImGui::Button(("Delete Group " + std::to_string(j)).c_str(), ImVec2(140, 20))) {
							pendingGroupDeletionWave = static_cast<int>(i);
							pendingGroupDeletion = static_cast<int>(j);
							showDeleteWarning = true;
							deleteGroup = true;
						}

						if (ImGui::Button("Save Changes", ImVec2(140, 20))) {
							SaveEnemyPopData();
						}

						ImGui::TreePop();
					}

					// Pop unique ID
					ImGui::PopID();
				}

				ImGui::Unindent();
			}

			// Pop unique ID
			ImGui::PopID();
		}

		ImGui::Separator();
		ImGui::Spacing();

		// 全データ保存ボタン
		if (ImGui::Button("Save All Data", ImVec2(200, 40))) {
			SaveEnemyPopData();
		}

	}
	ImGui::End();



	// 削除確認ダイアログ（ループ外で削除を実行）
	if (showDeleteWarning) {
		ImGui::OpenPopup("Confirm Deletion");
	}

	if (ImGui::BeginPopupModal("Confirm Deletion", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to delete this?");
		ImGui::Separator();

		if (deleteWave && pendingWaveDeletion != -1) {
			ImGui::Text("Wave %d will be deleted.", pendingWaveDeletion + 1);
		}
		if (deleteGroup && pendingGroupDeletion != -1 && pendingGroupDeletionWave != -1) {
			ImGui::Text("Group %d from Wave %d will be deleted.", pendingGroupDeletion, pendingGroupDeletionWave + 1);
		}

		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(80, 30))) {
			if (deleteWave && pendingWaveDeletion != -1) {
				waves_.erase(waves_.begin() + pendingWaveDeletion);
				SaveEnemyPopData();
				pendingWaveDeletion = -1;
			}
			if (deleteGroup && pendingGroupDeletion != -1 && pendingGroupDeletionWave != -1) {
				waves_[pendingGroupDeletionWave].groups.erase(waves_[pendingGroupDeletionWave].groups.begin() + pendingGroupDeletion);
				SaveEnemyPopData();
				pendingGroupDeletion = -1;
				pendingGroupDeletionWave = -1;
			}
			showDeleteWarning = false;
			deleteWave = false;
			deleteGroup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(80, 30))) {
			showDeleteWarning = false;
			deleteWave = false;
			deleteGroup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

