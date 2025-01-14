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

private:

    /// <summary>
    /// ImGui
    /// </summary>
    void ImGui();

    /// <summary>
    /// 敵を追加
    /// </summary>
    void AddEnemy();

    /// <summary>
    /// 敵を削除（デスフラグ付き）
    /// </summary>
    void RemoveDeadEnemies();

    /// <summary>
    /// 一定範囲内でランダム生成
    /// </summary>
    /// <returns></returns>
    Vector3 GenerateRandomPosition();

    /// <summary>
    /// 
    /// </summary>
    /// <param name="range"></param>
    void AttractEnemy(float range);

private:
    uint32_t enemyCount_ = 3;
    std::list<std::unique_ptr<Enemy>> enemies_;
    Camera* camera_ = nullptr;

    // ランダム生成範囲
    float randomRangeXMin_ = -10.0f;
    float randomRangeXMax_ = 10.0f;
    float randomRangeZMin_ = -10.0f;
    float randomRangeZMax_ = 10.0f;

    // 引き寄せ可能な範囲
    float typeRadius_ = 10.0f;
};
