#pragma once

#include <Framework/eScene/BaseScene.h>

#include <Rendering/Sprite/Sprite.h>

class LoadScene : public BaseScene
{
public:
    LoadScene() = default;
    ~LoadScene() = default;

    static std::unique_ptr<BaseScene> Create();

    void Initialize() override;
    void Update() override;
    void Draw() override;


private:

    Sprite* sprite_ = nullptr;




};