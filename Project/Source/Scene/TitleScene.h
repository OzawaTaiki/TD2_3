#pragma once
#include <Framework/eScene/BaseScene.h>
#include <memory>

#include <Rendering/LineDrawer/LineDrawer.h>
#include "Source/Objects/Entity/Enemy/TitleEnemy.h"

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

    std::array<std::unique_ptr<TitleEnemy>, 20> enemies_ = {};

#ifdef _DEBUG

    bool enemyMove_ = false;
    void ImGui();
#endif // _DEBUG
};