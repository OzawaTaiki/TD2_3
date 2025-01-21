#include "EnemyManager.h"
#include "../Player/Player.h"
#include "Rendering/LineDrawer/LineDrawer.h"
#include <Physics/Collision/CollisionManager.h>

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
    //TimeSpawnEnemy();






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
/// 2点間に生成したOBBのZ軸方向サイドに「None」属性の敵が触れているか判定し、
/// 触れている場合は押し出して最後は消滅させる処理を追加するサンプル実装例
/// </summary>
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
                std::powf(pos1.x - pos2.x, 2) +
                std::powf(pos1.y - pos2.y, 2) +
                std::powf(pos1.z - pos2.z, 2);

            float distance = std::sqrtf(distanceSquared);
            float radiiSum = range * 2.0f;
            if (distance > radiiSum) {
                continue; // 範囲外
            }

            // ------------------------------
            // 属性フラグ
            // ------------------------------
            bool isSameType = (enemy1->GetCurrentType() == enemy2->GetCurrentType());
            bool isTypeValid = (enemy1->GetCurrentType() != Enemy::BulletType::None &&
                enemy2->GetCurrentType() != Enemy::BulletType::None);

            // ------------------------------
            // 方向ベクトル (正規化)
            // ------------------------------
            Vector3 direction = { (pos2.x - pos1.x),
                                  (pos2.y - pos1.y),
                                  (pos2.z - pos1.z) };
            if (distance > 0.0f) {
                direction /= distance; // x,y,z すべて /distance
            }

            // ------------------------------
            // 反発 or 引き寄せ
            // ------------------------------
            if (isSameType && isTypeValid) {
                // 反発
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
                // 引き寄せ
                float attractForce = (attractCoefficient_ / distanceSquared);
                attractForce = (std::min)(attractForce, maxAttractForce_);

                pos1.x += direction.x * attractForce;
                pos1.y += direction.y * attractForce;
                pos1.z += direction.z * attractForce;

                pos2.x -= direction.x * attractForce;
                pos2.y -= direction.y * attractForce;
                pos2.z -= direction.z * attractForce;

                // 一定距離以下なら両者消滅
                if (distanceSquared <= std::powf(threshold_, 2)) {
                    enemy1->GetIsAlive() = false;
                    enemy2->GetIsAlive() = false;
                }

                // ------------------------------
                // North/Southを含むペア → AABB回転描画(OBB)
                // ------------------------------
                if ((enemy1->GetCurrentType() == Enemy::BulletType::North ||
                    enemy1->GetCurrentType() == Enemy::BulletType::South) ||
                    (enemy2->GetCurrentType() == Enemy::BulletType::North ||
                        enemy2->GetCurrentType() == Enemy::BulletType::South))
                {
                    // AABB算出
                    Vector3 center1 = enemy1->GetCenterPosition();
                    Vector3 center2 = enemy2->GetCenterPosition();

                    // 2点間の距離
                    Vector3 distance = center2 - center1;
                    // 方向ベクトル
                    Vector3 direction = distance.Normalize();
                    float length = (center2 - center1).Length();

                    // 2点間の中心
                    Vector3 center = center1 + (direction * length * 0.5f);

                    // OBBのサイズ xのみ変動
                    Vector3 size = { length - 2, 1.0f, 1.0f };
                    // ここで二点間の中心を中心とした真横のOBBができる
                    OBB obb(-size / 2.0f, size / 2.0f);

                    // OBBのワールド行列を求める
                    Matrix4x4 OBBWorldMat = Matrix4x4::Identity();

                    // scaleはデフォ{size}
                    Matrix4x4 scaleMat = MakeScaleMatrix(size);
                    // 中心点はcenter
                    Matrix4x4 translateMat = MakeTranslateMatrix(center);
                    // 回転を求める
                    Matrix4x4 rotMat = DirectionToDirection(Vector3(1, 0, 0), direction);

                    // がったい
                    OBBWorldMat = scaleMat * rotMat * translateMat;

                    // 頂点計算してもらう
                    obb.Calculate(OBBWorldMat);

#ifdef _DEBUG
                    // デバッグ時だけ描画
                    LineDrawer::GetInstance()->DrawOBB(OBBWorldMat);
#endif // _DEBUG

                    // OBBのサイズが一定以下のときに当たっている敵をマーク
                    // 引き寄せあってるやつらが消滅した時マークした敵を消す

                    const float kMaxSize = (std::max)(size.x, size.z);


                    // 衝突判定前に大きめサイズで事前チェック
                    // OBB中心 敵中心 で距離計算
                    // 一定範囲 (kMaxSize + enemySize ) * 2

                    for (auto it = enemies_.begin(); it != enemies_.end(); ++it)
                    {
                        Enemy* enemy = it->get();
                        // 既に消滅している敵 引き寄せあってる敵 マーク済みの敵 はスキップ
                        if (!enemy->GetIsAlive() || enemy == enemy1 || enemy == enemy2 || enemy->GetMarkForRemoval()) continue;

                            // 対象間距離を求める
                        Vector3 enemyCenter = enemy->GetCenterPosition();
                        Vector3 obb2enemydistance = enemyCenter - center;
                        float distanceLength = obb2enemydistance.Length();

                            // 一定範囲内にいる
                        if (distanceLength <= (kMaxSize + 1.0f) * 2.0f)
                        {
                            if (size.x <= kSizeThreshold)
                            {
                                if (CollisionManager::GetInstance()->IsCollision(obb, enemy->getcoll()->GetShape<OBB>()))
                                    enemy->GetIsAlive() = false;
                            }
                            // サイズがまだ大きいとき
                            else
                            {
                                // 敵より中心に近くないときはcontinue
                                Vector3 enemy1ToCenter = center - center1;
                                Vector3 enemy2ToCenter = center - center2;

                                Vector3 enemy1ToEnemy = enemyCenter - center1;
                                Vector3 enemy2ToEnemy = enemyCenter - center2;

                                // Dotで方向を求める
                                // どちらも＋なら敵の外側にいる
                                if (Dot(enemy1ToCenter.Normalize(), enemy1ToEnemy.Normalize()) < 0.0f ||
                                    Dot(enemy2ToCenter.Normalize(), enemy2ToEnemy.Normalize()) < 0.0f)
                                {
                                    continue;
                                }

                                if (CollisionManager::GetInstance()->IsCollision(enemy1->getcoll()->GetShape<OBB>(), enemy->getcoll()->GetShape<OBB>()))
                                {

                                    Vector3 trans = enemy->GetTranslate() - obb2enemydistance.Normalize() * attractForce;
                                    enemy->SetTranslate(trans);
                                }
                                else if (CollisionManager::GetInstance()->IsCollision(enemy2->getcoll()->GetShape<OBB>(), enemy->getcoll()->GetShape<OBB>()))
                                {

                                    Vector3 trans = enemy->GetTranslate() - obb2enemydistance.Normalize() * attractForce;
                                    enemy->SetTranslate(trans);
                                }
                            }
                        }
                    }
                }
            }

            // 計算後の位置を更新
            enemy1->SetTranslate(pos1);
            enemy2->SetTranslate(pos2);
        }
    }
}


void EnemyManager::RemoveMarkedEnemies()
{/*
    for (auto it : markedForRemovalEnemies_)
    {
        enemies_.erase(it);
    }
    markedForRemovalEnemies_.clear();*/
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