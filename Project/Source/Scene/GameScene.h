#pragma once
#include <Framework/eScene/BaseScene.h>

#include <Framework/Camera/Camera.h>
#include <Framework/Camera/DebugCamera.h>
#include <Rendering/Light/LightGroup.h>
#include <memory>

#include "../Objects/Entity/Player/Player.h"
#include "../Objects/Entity/Enemy/EnemyManager.h"
#include "../Camera/FollowCamera.h"
#include <Physics/Collision/CollisionManager.h>
#include "LoadScene.h"
    
#include <thread>

class GameScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

     ~GameScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    void Load();

    std::atomic<bool> Loading_ = true;
    std::unique_ptr<std::thread> loadThread_;

    Camera SceneCamera_ = {};
    DebugCamera debugCamera_ = {};
    FollowCamera followCamera_ = {};
    CollisionManager* collisionManager_ = nullptr;
    bool enableDebugCamera_ = false;

    std::unique_ptr<Player> player_ = nullptr;
    std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

    LightGroup lightGroup_ = {};

    LoadScene* loadScene_ = nullptr;

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG
};