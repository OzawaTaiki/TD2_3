#include "EnemyManager.h"
#include <imgui.h>
#include <random>

void EnemyManager::Initialize(Camera* camera)
{
    camera_ = camera;

    // 初期状態で敵を追加
    for (uint32_t i = 0; i < enemyCount_; ++i) {
        AddEnemy();
    }
}

void EnemyManager::Update()
{
    // 敵の更新
    for (auto& enemy : enemies_) {
        enemy->Update();
    }

    // デスフラグの立った敵を削除
    RemoveDeadEnemies();

    // 追加: ImGui操作用インターフェース
    if (ImGui::Begin("Enemy Info")) {
        ImGui::Text("Enemy Controls");
        ImGui::SliderFloat("X Range Min", &randomRangeXMin_, -50.0f, 0.0f);
        ImGui::SliderFloat("X Range Max", &randomRangeXMax_, 0.0f, 50.0f);
        ImGui::SliderFloat("Z Range Min", &randomRangeZMin_, -50.0f, 0.0f);
        ImGui::SliderFloat("Z Range Max", &randomRangeZMax_, 0.0f, 50.0f);

        if (ImGui::Button("Add Enemy")) {
            AddEnemy();
        }
    }
    ImGui::End();
}

void EnemyManager::Draw(const Vector4& color)
{
    // 敵の描画
    for (auto& enemy : enemies_) {
        enemy->Draw(color);
    }
}

void EnemyManager::AddEnemy()
{
    // 新しい敵を追加
    auto newEnemy = std::make_unique<Enemy>();
    Vector3 position = GenerateRandomPosition();
    newEnemy->Initialize(camera_);
    newEnemy->SetTranslate(position);
    enemies_.push_back(std::move(newEnemy));
}

void EnemyManager::RemoveDeadEnemies()
{
    enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
        return !enemy->GetIsAlive();
        });
}

Vector3 EnemyManager::GenerateRandomPosition()
{
    // 追加: メンバ変数を使用したランダム生成
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(randomRangeXMin_, randomRangeXMax_);
    std::uniform_real_distribution<float> distZ(randomRangeZMin_, randomRangeZMax_);
    return Vector3(distX(gen), 0.0f, distZ(gen));
}
