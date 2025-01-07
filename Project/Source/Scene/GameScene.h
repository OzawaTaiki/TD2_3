#pragma once
#include <Framework/eScene/BaseScene.h>

#include <Framework/Camera/Camera.h>
#include <Framework/Camera/DebugCamera.h>
#include <memory>

#include "../Objects/Player/Player.h"



class GameScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

     ~GameScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    Camera SceneCamera_ = {};
    DebugCamera debugCamera_ = {};
    bool enableDebugCamera_ = false;

    std::unique_ptr<Player> player_ = nullptr;

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG
};