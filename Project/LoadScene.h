#pragma once

#include <Framework/eScene/BaseScene.h>

#include <Rendering/Sprite/Sprite.h>

class LoadScene : public BaseScene
{
public:
    LoadScene() = default;
    ~LoadScene() = default;


    void Initialize() override;
    void Update() override;
    void Draw() override;


private:




};