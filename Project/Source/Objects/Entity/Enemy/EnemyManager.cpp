#include "EnemyManager.h"
#include "../Player/Player.h"
#include "Rendering/LineDrawer/LineDrawer.h"

#include <imgui.h>
#include <random>
#include <algorithm>

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

void EnemyManager::AttractEnemy(float range)
{
    for (auto it1 = enemies_.begin(); it1 != enemies_.end(); ++it1) {
        Enemy* enemy1 = it1->get();
        if (!enemy1 || !enemy1->GetIsAlive()) continue;

        for (auto it2 = std::next(it1); it2 != enemies_.end(); ++it2) {
            Enemy* enemy2 = it2->get();
            if (!enemy2 || !enemy2->GetIsAlive()) continue;

            /// ポジションを取得
            Vector3 pos1 = enemy1->GetTranslate();
            Vector3 pos2 = enemy2->GetTranslate();

            /*===============================================================//
                     　　	            敵同士の距離を計算
            //===============================================================*/
            float distanceSquared =
                std::pow(pos1.x - pos2.x, 2) +
                std::pow(pos1.y - pos2.y, 2) +
                std::pow(pos1.z - pos2.z, 2);

            float distance = std::sqrt(distanceSquared);
            float radiiSum = range * 2; /// 引き寄せまたは反発が作用する距離
            if (distance > radiiSum) {
                continue; /// 範囲外
            }

            /*===============================================================//
                     　　	       属性を確認するためのフラグ
            //===============================================================*/
            bool isSameType = enemy1->GetCurrentType() == enemy2->GetCurrentType();
            bool isTypeValid = enemy1->GetCurrentType() != Enemy::BulletType::None && enemy2->GetCurrentType() != Enemy::BulletType::None;

           


            /*===============================================================//
         　　	                        方向の計算
            //===============================================================*/

            /// 力の方向を計算
            Vector3 direction = {
                (pos2.x - pos1.x),
                (pos2.y - pos1.y),
                (pos2.z - pos1.z)
            };

            /// 中心点を求めます
            Vector3 centerpos = direction / 2.0f;


            /// ベクトルの正規化
            if (distance > 0.0f) {
                direction.x /= distance;
                direction.y /= distance;
                direction.z /= distance;
            }

           





            /*===============================================================//
　　	                                 各属性ごとの判定
            //===============================================================*/



            /// 同じ属性かつ有効なタイプの場合: 強い反発（距離依存）
            /// 異なる属性の場合: 強く引き寄せ（距離依存）
            if (isSameType && isTypeValid) {

                float repelForce = (repelCoefficient_ / distanceSquared); /// 距離の2乗に反比例
                repelForce = (std::min)(repelForce, maxRepelForce_); /// 最大反発力を制限

                pos1.x -= direction.x * repelForce;
                pos1.y -= direction.y * repelForce;
                pos1.z -= direction.z * repelForce;

                pos2.x += direction.x * repelForce;
                pos2.y += direction.y * repelForce;
                pos2.z += direction.z * repelForce;

            }
            else if (!isSameType && isTypeValid) {

                float attractForce = (attractCoefficient_ / distanceSquared); /// 距離の2乗に反比例
                attractForce = (std::min)(attractForce, maxAttractForce_); /// 最大引き寄せ力を制限

                pos1.x += direction.x * attractForce;
                pos1.y += direction.y * attractForce;
                pos1.z += direction.z * attractForce;

                pos2.x -= direction.x * attractForce;
                pos2.y -= direction.y * attractForce;
                pos2.z -= direction.z * attractForce;

                /*===============================================================//
                     　　	                  消滅
                //===============================================================*/
                if (distanceSquared <= std::pow(threshold_, 2)) {
                    enemy1->GetIsAlive() = false;
                    enemy2->GetIsAlive() = false;
                }
            }

            // 新しい位置を更新
            enemy1->SetTranslate(pos1);
            enemy2->SetTranslate(pos2);
        }
    }
}

