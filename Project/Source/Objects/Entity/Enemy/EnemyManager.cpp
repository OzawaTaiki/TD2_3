#include "EnemyManager.h"

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
    newEnemy->Initialize(camera_);  // カメラを渡して初期化
    enemies_.push_back(std::move(newEnemy));
}

void EnemyManager::RemoveDeadEnemies()
{
    // デスフラグが立っている敵を削除
    enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
        return !enemy->GetIsAlive();  
        });
}
