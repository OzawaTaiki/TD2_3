#include "TitleScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>
#include <Physics/Collision/CollisionManager.h>

std::unique_ptr<BaseScene> TitleScene::Create()
{
	return std::make_unique<TitleScene>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_.y = 45;
    SceneCamera_.translate_.z = -1.5f;

    SceneCamera_.rotate_.x = std::numbers::pi_v<float> / 2.0f;

    lineDrawer_ = LineDrawer::GetInstance();
    lineDrawer_->Initialize();
    lineDrawer_->SetCameraPtr(&SceneCamera_);


    InitJson();

    InitEnemy();

    player_ = std::make_unique<Player>();
    player_->Initialize(&SceneCamera_);

    BG_.Initialize("TItleBG");

    titleUI_ = std::make_unique<TItleUI>();
    titleUI_->Initialize();

    DirectionalLight DLight{};
    DLight.direction = Vector3(-0.909f, -0.417f, 0.0f).Normalize();
    DLight.intensity = 1.0f;

    lg.Initialize();
    lg.SetDirectionalLight(DLight);

    titleLogo_ = std::make_unique<ObjectModel>();
    titleLogo_->Initialize("TitleLogo/title.obj", "title");
    float pi = std::numbers::pi_v<float>;

    titleLogo_->translate_ = { 0,-1,4 };
    titleLogo_->scale_ = { 4,4,1 };
    titleLogo_->rotate_ = { pi / 2.0f,0,pi };


    LightingSystem::GetInstance()->SetLightGroup(&lg);

    audio_ = AudioSystem::GetInstance();

}

void TitleScene::Update()
{

    CollisionManager::GetInstance()->ResetColliderList();

    //if(Input::GetInstance()->IsKeyTriggered(DIK_TAB) ||
    //   Input::GetInstance()->IsPadTriggered(PadButton::iPad_A))
    //{
    //    SceneManager::GetInstance()->ReserveScene("Game");
    //}

    titleLogo_->Update();
    player_->Update();

#ifdef _DEBUG
    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN))
    {
        enemyMove_ = !enemyMove_;
    }

    if (ImGui::Begin("Title"))
    {
        ImGui::DragFloat("EnemySpeed", &enemySpeed_, 0.01f);
        ImGui::DragFloat("EnemyRange", &enemySpawnRange_, 0.01f);

        ImGui::Checkbox("move", &enemyMove_);

        if (ImGui::Button("Set"))
        {
            InitEnemy();
        }

        if (ImGui::Button("Save"))
        {
            titleSceneSetting_->Save();
        }
        ImGui::End();
    }

    BG_.Update();
    titleUI_->Update();


    //if (enemyMove_)
#endif // _DEBUG

    for (auto it = enemies_.begin(); it != enemies_.end();)
    {
        auto& enemy = *it;
            enemy->Update();
            if (enemy->GetIsAlive())
                ++it;
            else
                it = enemies_.erase(it);
    }

    if(enemyManager_->AttractEnemy(enemies_))
    {
        SceneManager::ReserveScene("Game");
    }


    SceneCamera_.Update();
    SceneCamera_.UpdateMatrix();

    player_->SetHp(100.0f);

    CollisionManager::GetInstance()->CheckAllCollision();
}

void TitleScene::Draw()
{
    Sprite::PreDraw();
    BG_.Draw();

    titleLogo_->Draw(&SceneCamera_, { 1,1,1,1 });

    player_->Draw({ 0,0,0,1 });

    for (auto& enemy : enemies_)
    {
        enemy->Draw({ 0,0,0,1 });
    }

    Sprite::PreDraw();
    titleUI_->Draw();

    lineDrawer_->Draw();
}
void TitleScene::InitJson()
{
    titleSceneSetting_ = std::make_unique<JsonBinder>("TitleScene", "Resources/Data/Parameter");

    titleSceneSetting_->RegisterVariable("enemyRange", &enemySpawnRange_);
    titleSceneSetting_->RegisterVariable("enemySpeed", &enemySpeed_);

    jsonBinder_ = std::make_unique<JsonBinder>("enemyManager", "Resources/Data/Parameter");

    jsonBinder_->RegisterVariable("hitSound_Volume", &hitVolume_);
    jsonBinder_->RegisterVariable("deathSound_Volume", &deathVolume_);
}

void TitleScene::InitEnemy()
{
    uint32_t hitHandle = AudioSystem::GetInstance()->SoundLoadWave("hit.wav");
    uint32_t deathHandle = AudioSystem::GetInstance()->SoundLoadWave("knockdown.wav");

    enemies_.clear();

    float posMin = 20.0f;

    float div = std::ceilf(posMin / enemySpawnRange_);
    posMin = div * enemySpawnRange_;

    const size_t enemyNum = static_cast<uint32_t>(posMin / enemySpawnRange_ * 2);

    for (size_t i = 0; i < enemyNum; i++)
    {
        auto& enemy = enemies_.emplace_back(std::make_unique<TitleEnemy>());
        enemy->Initialize(&SceneCamera_);
        enemy->SetTranslate(Vector3{ static_cast<float>(-posMin + i * enemySpawnRange_),0,0 });
        enemy->SetRangeOfMovement({ posMin,0.0f,0.0f }, { -posMin,0.0f,0.0f });
        enemy->SetMoveSpeed(enemySpeed_);
        enemy->SetHitSound(hitHandle, hitVolume_, 0.0f);
        enemy->SetDeathSound(deathHandle, deathVolume_, 0.0f);
    };

    enemyManager_ = std::make_unique<EnemyManager>();
}

#ifdef _DEBUG
void TitleScene::ImGui()
{
}
#endif // DEBUG
