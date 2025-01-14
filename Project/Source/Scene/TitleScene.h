#pragma once
#include <Framework/eScene/BaseScene.h>
#include <memory>

#include <Rendering/LineDrawer/LineDrawer.h>

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

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG
};