#include "EnemyManager.h"
#include "../Player/Player.h"
#include "Rendering/LineDrawer/LineDrawer.h"
#include <Physics/Collision/CollisionManager.h>

#include <imgui.h>
#include <random>
#include <algorithm>
#include <Physics/Math/MatrixFunction.h>
#include <Physics/Math/VectorFunction.h>
#include <ResourceManagement/TextureManager/TextureManager.h>


void EnemyManager::Initialize(Camera* camera)
{
    camera_ = camera;


    // コールバック関数の登録
	spawnJson_.SetPlayer(player_);
    spawnJson_.SetSpawnCallback([this](Vector3& position, float& speed,Vector3& goal,std::string& moveType) {
        this->SpawnEnemy(position,speed,goal, moveType);
        });

    spawnJson_.SetWarningCallback([this](Vector3& position, float warningTime) {
        this->SpawnWarning(position, warningTime);
        });


    // スポーンデータの読み込み
    spawnJson_.LoadEnemyPopData();


    gameTime_ = GameTime::GetInstance();

    hitHandle_ = AudioSystem::GetInstance()->SoundLoadWave("hit.wav");
    deathHandle_ = AudioSystem::GetInstance()->SoundLoadWave("knockdown.wav");

    InitJsonBinder();

}

void EnemyManager::Update()
{

    //gameTime_->GetChannel("default").SetGameSpeed(5.0f);
    float deltaTime = gameTime_->GetChannel("default").GetDeltaTime<float>();
    spawnJson_.UpdateEnemyPopCommands();

    /// デスフラグの立った敵を削除
    RemoveDeadEnemies();
    AttractEnemy(attractRadius_);

    /// 敵の更新
    for (auto& enemy : enemies_) {
		enemy->SetGameScene(gameScene_);
        enemy->Update();
        enemy->Move(deltaTime);
        enemy->ChangeType(deltaTime);
    }

    // 警告モデルの更新
    for (auto it = warningModels_.begin(); it != warningModels_.end();) {
        it->oModel->Update();
        it->timer -= deltaTime;

        // 回転アニメーション
        it->oModel->rotate_ = warningRotate_;/** deltaTime*/;
        it->oModel->translate_.y += warningRiseSpeed_ * deltaTime;
        it->oModel->scale_ += warningScaleSpeed_ * deltaTime;
        warningColor.w = (std::min)(warningColor.w + spawnJson_.GetSpawnWarningTime()* deltaTime, setColor.w);
        if (it->timer <= 0.0f) {
            it = warningModels_.erase(it);
            warningColor.w = 0.0f;
        }
        else {
            ++it;
        }
    }

    for (auto it = deathEffects_.begin(); it != deathEffects_.end();)
    {
        (*it)->Update();
        if ((*it)->IsEnd())
        {
            it = deathEffects_.erase(it);
        }
        else
        {
            ++it;
        }
    }

#ifdef _DEBUG
    ImGui();
    spawnJson_.ShowImGui(); // 追加
#endif
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

    // 警告モデルの描画
    for (const auto& warning : warningModels_) {
        warning.oModel->Draw(camera_, warningColor);  // 半透明の赤色で描画
    }
}

void EnemyManager::DrawSprite()
{
    for (auto& enemy : enemies_) {
        enemy->DrawSprite();
    }
}

void EnemyManager::DrawDeathEffect()
{
    for (auto& effect : deathEffects_)
    {
        effect->Draw(camera_);
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

        ImGui::SeparatorText("Sound");
        ImGui::Text("Hit Sound");
        ImGui::SliderFloat("Hit Sound Volume", &hitVolume_, 0.0f, 1.0f);
        ImGui::DragFloat("Hit Sound Offset", &hitStartOffset_, 0.01f, 0.0f);
        ImGui::Text("Death Sound");
        ImGui::SliderFloat("Death Sound Volume", &deathVolume_, 0.0f, 1.0f);
        ImGui::DragFloat("Death Sound Offset", &deathStartOffset_, 0.01f, 0.0f);



        ImGui::SeparatorText("Warning Model Animation");
        ImGui::DragFloat3("Rotate Speed", &warningRotate_.x, 1.0f, 50.0f);
        ImGui::DragFloat("Rise Speed", &warningRiseSpeed_, 1.0f, 20.0f);
        ImGui::DragFloat("Scale Speed", &warningScaleSpeed_, 1.0f, 20.0f);
		ImGui::DragFloat4("Color", &setColor.x, 0.01f, 0.0f, 1.0f);

        if (ImGui::Button("Set"))
        {
            for (auto& enemy : enemies_) {
                enemy->SetHitSound(hitHandle_, hitVolume_, hitStartOffset_);
                enemy->SetDeathSound(deathHandle_, deathVolume_, deathStartOffset_);
            }
        }
        if (ImGui::Button("Save"))
        {
            jsonBinder_->Save();
        }

    }
    ImGui::End();
}




void EnemyManager::SpawnEnemy(Vector3& position, float& speed, Vector3& goal, std::string& moveType)
{
    auto newEnemy = std::make_unique<Enemy>();
    newEnemy->Initialize(camera_);
    newEnemy->SetTranslate(position);
    newEnemy->SetSpeed(speed);
    newEnemy->SetGoal(goal);
    newEnemy->SetPlayer(player_);
    newEnemy->SetMoveType(moveType);
    newEnemy->SetHitSound(hitHandle_, hitVolume_, hitStartOffset_);
    newEnemy->SetDeathSound(deathHandle_, deathVolume_, deathStartOffset_);
    enemies_.push_back(std::move(newEnemy));
}

void EnemyManager::SpawnWarning(Vector3& position, float& warningTime)
{
    SpawnWarningModel warning;
    warning.oModel = std::make_unique<ObjectModel>();
    warning.oModel->Initialize("plane/plane.obj", "Warning");
    warning.oModel->translate_ = position;
    warning.timer = warningTime;
    warning.position = position;
    warningModels_.push_back(std::move(warning));
}

void EnemyManager::RemoveDeadEnemies()
{
    for (auto it = enemies_.begin(); it != enemies_.end();)
    {
        (*it)->Update();
        if (!(*it)->GetIsAlive() || !(*it)->GetIsDraw())
        {
            if (!effectPos_.empty())
            {
                auto& effect = deathEffects_.emplace_back(std::make_unique<EnemyDeathEffect>());
                effect->Initialize(std::get<2>(effectPos_.front()));
                effectPos_.pop_front();
            }
            it = enemies_.erase(it);
            continue;
        }
        ++it;
    }
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

                bool makeTuple = false;

                // 一定距離以下なら両者消滅
                if (distanceSquared <= std::powf(threshold_, 2)) {
                    enemy1->SetIsAlive(false);
                    enemy2->SetIsAlive(false);

                    enemy1->SetIsDraw(false);
                    enemy2->SetIsDraw(false);

                    makeTuple = true;
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
                    Vector3 size = { length - 2, 1.0f, 2.0f };
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

                    if (makeTuple)
                    {
                        EffectPos ep = std::make_tuple(it1, it2, center);
                        if (std::find(effectPos_.begin(), effectPos_.end(), ep) == effectPos_.end())
                        {
                            effectPos_.push_back(ep);
                        }
                    }

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
                                if (CollisionManager::GetInstance()->IsCollision(obb, enemy->getcoll()->GetShape<OBB>())) {
                                    enemy->SetIsAlive(false);
									enemy->SetIsDraw(false);
                                }
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

bool EnemyManager::AttractEnemy(std::list<std::unique_ptr<TitleEnemy>>& _enemies, float range) const
{
    bool flag = false;

    for (auto it1 = _enemies.begin(); it1 != _enemies.end(); ++it1) {
        Enemy* enemy1 = it1->get();
        if (!enemy1 || !enemy1->GetIsAlive()) continue;

        for (auto it2 = std::next(it1); it2 != _enemies.end(); ++it2) {
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
                    enemy1->SetIsAlive(false);
                    enemy2->SetIsAlive(false);

                    enemy1->SetIsDraw(false);
                    enemy2->SetIsDraw(false);

                    flag = true;
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
                    Vector3 size = { length - 2, 1.0f, 2.0f };
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

                    for (auto it = _enemies.begin(); it != _enemies.end(); ++it)
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
                                if (CollisionManager::GetInstance()->IsCollision(obb, enemy->getcoll()->GetShape<OBB>())) {
                                    enemy->SetIsAlive(false);
                                    enemy->SetIsDraw(false);
                                }
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

    return flag;
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

void EnemyManager::InitJsonBinder()
{
    jsonBinder_ = std::make_unique<JsonBinder>("enemyManager","Resources/Data/Parameter");

    jsonBinder_->RegisterVariable("hitSound_Volume", &hitVolume_);
    jsonBinder_->RegisterVariable("hitSound_Offset", &hitStartOffset_);

    jsonBinder_->RegisterVariable("deathSound_Volume", &deathVolume_);
    jsonBinder_->RegisterVariable("deathSound_Offset", &deathStartOffset_);

    jsonBinder_->RegisterVariable("warning_RotateSpeed", &warningRotate_);
    jsonBinder_->RegisterVariable("warning_RiseSpeed", &warningRiseSpeed_);
    jsonBinder_->RegisterVariable("warning_ScaleSpeed", &warningScaleSpeed_);
	jsonBinder_->RegisterVariable("SetColor", &setColor);
}
