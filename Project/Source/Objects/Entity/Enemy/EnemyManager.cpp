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
    // デスフラグの立った敵を削除
    RemoveDeadEnemies();

    // 敵の更新
    for (auto& enemy : enemies_) {
        enemy->Update();
    }

    AttractEnemy(typeRadius_);


    

#ifdef _DEBUG
    ImGui();
#endif // _DEBUG

}

void EnemyManager::Draw(const Vector4& color)
{
    // 敵の描画
    for (auto& enemy : enemies_) {
        enemy->Draw(color);
    }
}

void EnemyManager::ImGui()
{
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

void EnemyManager::AttractEnemy(float range)
{
    for (auto it1 = enemies_.begin(); it1 != enemies_.end(); ++it1) {
        Enemy* enemy1 = it1->get();
        if (!enemy1 || !enemy1->GetIsAlive()) continue;

        for (auto it2 = std::next(it1); it2 != enemies_.end(); ++it2) {
            Enemy* enemy2 = it2->get();
            if (!enemy2 || !enemy2->GetIsAlive()) continue;

            // 弾のタイプが正負でない場合はスキップ
            if ((enemy1->GetCurrentType() == Enemy::BulletType::None) ||
                (enemy2->GetCurrentType() == Enemy::BulletType::None)) {
                continue;
            }

            // 同じ弾のタイプの場合はスキップ
            if (enemy1->GetCurrentType() == enemy2->GetCurrentType()) {
                continue;
            }

            // 円形当たり判定: 距離がそれぞれの半径の合計以下か判定
            Vector3 pos1 = enemy1->GetTranslate();
            Vector3 pos2 = enemy2->GetTranslate();
            float radius1 = range; // Enemy1の半径
            float radius2 = range; // Enemy2の半径

            float distanceSquared =
                std::pow(pos1.x - pos2.x, 2) +
                std::pow(pos1.y - pos2.y, 2) +
                std::pow(pos1.z - pos2.z, 2);

            float radiiSum = radius1 + radius2;
            if (distanceSquared <= std::pow(radiiSum, 2)) {
                // 引き寄せ開始
                Vector3 direction = {
                    (pos2.x - pos1.x),
                    (pos2.y - pos1.y),
                    (pos2.z - pos1.z)
                };

                // 移動処理
                float moveSpeed = 0.1f; // 移動速度
                pos1.x += direction.x * moveSpeed;
                pos1.y += direction.y * moveSpeed;
                pos1.z += direction.z * moveSpeed;

                pos2.x -= direction.x * moveSpeed;
                pos2.y -= direction.y * moveSpeed;
                pos2.z -= direction.z * moveSpeed;

                enemy1->SetTranslate(pos1);
                enemy2->SetTranslate(pos2);

                // 引き寄せ後、位置が一致したら消滅
                if (std::abs(pos1.x - pos2.x) < 0.01f &&
                    std::abs(pos1.y - pos2.y) < 0.01f &&
                    std::abs(pos1.z - pos2.z) < 0.01f) {
                    enemy1->GetIsAlive() = false;
                    enemy2->GetIsAlive() = false;
                }
            }
        }
    }
}


