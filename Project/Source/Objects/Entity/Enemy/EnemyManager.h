#pragma once
#include <list>
#include <memory>
#include "Enemy.h"
#include <Framework/Camera/Camera.h>

class EnemyManager
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Camera* camera);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw(const Vector4& color);

    /// <summary>
    /// 敵を追加
    /// </summary>
    void AddEnemy();

    /// <summary>
    /// 敵を削除（デスフラグ付き）
    /// </summary>
    void RemoveDeadEnemies();

private:
    uint32_t enemyCount_ = 1;
    std::list<std::unique_ptr<Enemy>> enemies_;
    Camera* camera_ = nullptr;
};
