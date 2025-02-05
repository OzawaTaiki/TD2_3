#pragma once
#include <Framework/eScene/BaseScene.h>
#include <memory>

#include <Rendering/LineDrawer/LineDrawer.h>
#include "Source/Objects/Entity/Enemy/TitleEnemy.h"
#include "Source/Objects/Entity/Enemy/EnemyManager.h"
#include "Source/Objects/Entity/Player/Player.h"
#include <Systems/JsonBinder/JsonBinder.h>
#include "Source/TitleUI/TItleUI.h"
#include <Systems/Audio/AudioSystem.h>

class TitleScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    ~TitleScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:

    void InitJson();

    void InitEnemy();

    Camera SceneCamera_ = {};
    LineDrawer* lineDrawer_ = nullptr;

    UISprite BG_ = {};

    std::list<std::unique_ptr<TitleEnemy>> enemies_ = {};
    std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

    std::unique_ptr<TItleUI> titleUI_ = nullptr;

    std::unique_ptr<Player> player_ = nullptr;

    std::unique_ptr<ObjectModel> titleLogo_ = nullptr;

    AudioSystem* audio_;

    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;
    float hitVolume_ = 0;
    float deathVolume_ = 0;

    std::unique_ptr<JsonBinder> titleSceneSetting_ = nullptr;

    float enemySpawnRange_ = 4.0f;
    float enemySpeed_ = 0.07f;

    size_t enemyNum_ = 0;
    float posMin = 20.0f;

    uint32_t hitHandle;
    uint32_t deathHandle;

    float currentTime_ = 0;
    float spawnInterval_ = 1.0f;

    // 行動制限
    Vector3 min = { -10.0f,0.0f,-10.0f };
    Vector3 max = { 10.0f,0.0f,10.0f };


    LightGroup lg = {};

#ifdef _DEBUG

    bool enemyMove_ = false;
    void ImGui();
#endif // _DEBUG
};