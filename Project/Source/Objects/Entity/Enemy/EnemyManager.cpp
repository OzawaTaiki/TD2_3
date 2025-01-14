#include "EnemyManager.h"

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
    // ランダムな初期位置を設定
    Vector3 position = GenerateRandomPosition();
    newEnemy->Initialize(camera_);
    newEnemy->SetTranslate(position);
    enemies_.push_back(std::move(newEnemy));
}

void EnemyManager::RemoveDeadEnemies()
{
    // デスフラグが立っている敵を削除
    enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
        return !enemy->GetIsAlive();  
        });
}

Vector3 EnemyManager::GenerateRandomPosition()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> distX(-10.0f, 10.0f); // X座標範囲
    static std::uniform_real_distribution<float> distY(0.0f, 0.0f);     // Y座標範囲
    static std::uniform_real_distribution<float> distZ(-10.0f, 10.0f); // Z座標範囲

    return Vector3(distX(gen), distY(gen), distZ(gen));
}