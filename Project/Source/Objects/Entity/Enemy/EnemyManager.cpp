#include "EnemyManager.h"
#include "../Player/Player.h"
#include "Rendering/LineDrawer/LineDrawer.h"

#include <imgui.h>
#include <random>
#include <algorithm>
#include <Physics/Math/MatrixFunction.h>
#include <Physics/Math/VectorFunction.h>

void EnemyManager::Initialize(Camera* camera)
{
    camera_ = camera;

    for (uint32_t i = 0; i < enemyCount_; ++i) {
        AddEnemy();
    }

    lastSpawnTime_ = std::chrono::steady_clock::now();
    spawnInterval_ = 1.5f; // 1.5秒ごとに敵を生成
}

void EnemyManager::Update()
{
    /// デスフラグの立った敵を削除
    RemoveDeadEnemies();

    /// 敵の更新
    for (auto& enemy : enemies_) {
        enemy->Update();
    }

    AttractEnemy(attractRadius_);

    /// 一定時間で敵追加
    TimeSpawnEnemy();






#ifdef _DEBUG
    ImGui();
#endif /// _DEBUG

}

void EnemyManager::Draw(const Vector4& color)
{
    /// 敵の描画
    for (auto& enemy : enemies_) {
        enemy->Draw(color);
        if (enemy->GetCurrentTypeName() != "None") {
            LineDrawer::GetInstance()->DrawCircle(enemy->GetTranslate(), attractRadius_);
        }
    }
   
    



}

void EnemyManager::ImGui()
{
    if (ImGui::Begin("Enemy Info")) {

        ImGui::Text("Enemy Controls");
        ImGui::SliderFloat("X Range Min", &randomRangeXMin_, -50.0f, 0.0f);
        ImGui::SliderFloat("X Range Max", &randomRangeXMax_, 0.0f, 50.0f);
        ImGui::SliderFloat("Z Range Min", &randomRangeZMin_, -50.0f, 0.0f);
        ImGui::SliderFloat("Z Range Max", &randomRangeZMax_, 0.0f, 50.0f);

        ImGui::Text("Enemy Attract");
        ImGui::SliderFloat("Attract Speed", &attractSpeed_, 0.0f, 0.5f);
        ImGui::SliderFloat("Attract Radius", &attractRadius_, 0.0f, 10.0f);
        ImGui::SliderFloat("Deth Distance", &threshold_, 0.0f, 2.0f);
        ImGui::SliderFloat("Repel Coefficient", &repelCoefficient_, 0.0f, 10.0f);
        ImGui::SliderFloat("Attract Coefficient", &attractCoefficient_, 0.0f, 10.0f);
        ImGui::SliderFloat("Max Repel Force", &maxRepelForce_, 0.0f, 10.0f);
        ImGui::SliderFloat("Max Attract Force", &maxAttractForce_, 0.0f, 10.0f);


        if (ImGui::Button("Add Enemy")) {
            AddEnemy();
        }
    }
    ImGui::End();
}

void EnemyManager::AddEnemy()
{
    /// 新しい敵を追加
    auto newEnemy = std::make_unique<Enemy>();
    Vector3 randomPos= GenerateRandomPosition();
    Vector3 playerPos = player_->GetCenterPosition();
    Vector3 newPos = randomPos + playerPos;

    newEnemy->Initialize(camera_);
    newEnemy->SetTranslate(newPos);
    enemies_.push_back(std::move(newEnemy));
}

void EnemyManager::TimeSpawnEnemy()
{
    /// 時間に基づいて敵を追加
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastSpawnTime_;
    if (elapsed.count() >= spawnInterval_) {
        AddEnemy();
        lastSpawnTime_ = currentTime; /// タイマーをリセット
    }
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

/// <summary>
/// AttractEnemy
/// </summary>
/// <param name="range">敵同士が影響を及ぼし合う範囲</param>
void EnemyManager::AttractEnemy(float range)
{
    for (auto it1 = enemies_.begin(); it1 != enemies_.end(); ++it1) {
        Enemy* enemy1 = it1->get();
        if (!enemy1 || !enemy1->GetIsAlive()) continue;

        for (auto it2 = std::next(it1); it2 != enemies_.end(); ++it2) {
            Enemy* enemy2 = it2->get();
            if (!enemy2 || !enemy2->GetIsAlive()) continue;

            // ------------------------------
            // 距離計算
            // ------------------------------
            Vector3 pos1 = enemy1->GetTranslate();
            Vector3 pos2 = enemy2->GetTranslate();

            float distanceSquared =
                std::pow(pos1.x - pos2.x, 2) +
                std::pow(pos1.y - pos2.y, 2) +
                std::pow(pos1.z - pos2.z, 2);
            float distance = std::sqrt(distanceSquared);

            float radiiSum = range * 2.0f;
            if (distance > radiiSum) {
                // 範囲外ならスキップ
                continue;
            }

            // ------------------------------
            // 属性のフラグ
            // ------------------------------
            bool isSameType = (enemy1->GetCurrentType() == enemy2->GetCurrentType());
            bool isTypeValid = (enemy1->GetCurrentType() != Enemy::BulletType::None &&
                enemy2->GetCurrentType() != Enemy::BulletType::None);

            // ------------------------------
            // 方向の計算 (正規化)
            // ------------------------------
            Vector3 direction = {
                (pos2.x - pos1.x),
                (pos2.y - pos1.y),
                (pos2.z - pos1.z)
            };
            if (distance > 0.0f) {
                direction.x /= distance;
                direction.y /= distance;
                direction.z /= distance;
            }

            // ------------------------------
            // 同じなら反発、異なるなら引き寄せ
            // ------------------------------
            if (isSameType && isTypeValid) {
                float repelForce = (repelCoefficient_ / distanceSquared);
                repelForce = (std::min)(repelForce, maxRepelForce_);

                pos1.x -= direction.x * repelForce;
                pos1.y -= direction.y * repelForce;
                pos1.z -= direction.z * repelForce;

                pos2.x += direction.x * repelForce;
                pos2.y += direction.y * repelForce;
                pos2.z += direction.z * repelForce;
            }
            else if (!isSameType && isTypeValid) {
                float attractForce = (attractCoefficient_ / distanceSquared);
                attractForce = (std::min)(attractForce, maxAttractForce_);

                pos1.x += direction.x * attractForce;
                pos1.y += direction.y * attractForce;
                pos1.z += direction.z * attractForce;

                pos2.x -= direction.x * attractForce;
                pos2.y -= direction.y * attractForce;
                pos2.z -= direction.z * attractForce;

                // ある閾値内なら両者消滅
                if (distanceSquared <= std::pow(threshold_, 2)) {
                    enemy1->GetIsAlive() = false;
                    enemy2->GetIsAlive() = false;
                }

                // ------------------------------
                // North/South を含むペアだけ AABB→回転描画(OBB)
                // ------------------------------
                if ((enemy1->GetCurrentType() == Enemy::BulletType::North ||
                    enemy1->GetCurrentType() == Enemy::BulletType::South) ||
                    (enemy2->GetCurrentType() == Enemy::BulletType::North ||
                        enemy2->GetCurrentType() == Enemy::BulletType::South))
                {
                    // AABB の min/max を計算
                    Vector3 center1 = enemy1->GetCenterPosition();
                    Vector3 center2 = enemy2->GetCenterPosition();

                    float minX = (std::min)(center1.x, center2.x);
                    float maxX = (std::max)(center1.x, center2.x);
                    float minY = (std::min)(center1.y, center2.y);
                    float maxY = (std::max)(center1.y, center2.y);
                    float minZ = (std::min)(center1.z, center2.z);
                    float maxZ = (std::max)(center1.z, center2.z);

                    // AABB 8頂点
                    Vector3 corners[8] = {
                        { minX, minY, minZ }, // 0
                        { maxX, minY, minZ }, // 1
                        { minX, maxY, minZ }, // 2
                        { maxX, maxY, minZ }, // 3,
                        { minX, minY, maxZ }, // 4
                        { maxX, minY, maxZ }, // 5
                        { minX, maxY, maxZ }, // 6
                        { maxX, maxY, maxZ }, // 7
                    };

                    // AABB中心
                    Vector3 obbCenter = {
                        (minX + maxX) * 0.5f,
                        (minY + maxY) * 0.5f,
                        (minZ + maxZ) * 0.5f
                    };

                    // ------------------------------
                    // 回転行列を作成:
                    // DirectionToDirection( from, to )
                    //
                    // 例として "Z軸(0,0,1)" → "(center2 - center1).Normalize()"
                    // ------------------------------
                    Vector3 from = Vector3(1, 0, 0);
                    Vector3 to = (center2 - center1).Normalize();

                    ///
                    /// 
                    /// 

                    // 万が一 zeroベクトルならスキップするなど
                    float aiai = Length(to);
                    if (aiai < 1e-6f) {
                        // ここでは何もしない
                    }

                    Matrix4x4 rotMatrix = DirectionToDirection(from, to);

                    // ------------------------------
                    // 回転後の頂点を入れる配列
                    // ------------------------------
                    Vector3 rotated[8];

                    // 各頂点に回転を適用 (中心を原点に→行列適用→戻す)
                    for (int i = 0; i < 8; i++) {
                        // 中心を引く
                        Vector3 localPos = corners[i] - obbCenter;

                        // 4次元に拡張(w=1.0)
                        Vector4 localPos4(localPos.x, localPos.y, localPos.z, 1.0f);

                        // rotMatrixを掛け算 (環境に合わせて実装)
                        Vector4 out4 = Vector4ooooo(rotMatrix, localPos4);

                        // 3次元に戻す
                        rotated[i] = Vector3(out4.x, out4.y, out4.z);

                        // 再度中心を足してワールド座標へ
                        rotated[i] += obbCenter;
                    }

                    // ------------------------------
                    // ライン描画で OBB を可視化
                    // ------------------------------
                    // 下面
                    LineDrawer::GetInstance()->RegisterPoint(rotated[0], rotated[1]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[0], rotated[2]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[1], rotated[3]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[2], rotated[3]);

                    // 上面
                    LineDrawer::GetInstance()->RegisterPoint(rotated[4], rotated[5]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[4], rotated[6]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[5], rotated[7]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[6], rotated[7]);

                    // 側面
                    LineDrawer::GetInstance()->RegisterPoint(rotated[0], rotated[4]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[1], rotated[5]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[2], rotated[6]);
                    LineDrawer::GetInstance()->RegisterPoint(rotated[3], rotated[7]);

                    // ------------------------------
                    // (必要であれば) 回転OBB内の None 敵を探す…など
                    // ------------------------------
                    // ※ OBB上での厳密な内外判定は、
                    //    さらに逆行列でローカル空間に変換する等が必要
                }
            }

            // 計算後の位置を更新
            enemy1->SetTranslate(pos1);
            enemy2->SetTranslate(pos2);
        }
    }
}

Vector4 EnemyManager::Vector4ooooo(const Matrix4x4& m, const Vector4& v)
{
    Vector4 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}