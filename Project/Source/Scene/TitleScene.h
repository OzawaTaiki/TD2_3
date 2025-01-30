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

    LightGroup lg = {};

#ifdef _DEBUG

    bool enemyMove_ = false;
    void ImGui();
#endif // _DEBUG
};