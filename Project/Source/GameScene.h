#pragma once
#include <Framework/eScene/BaseScene.h>
#include <memory>

class GameScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

     ~GameScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG
};