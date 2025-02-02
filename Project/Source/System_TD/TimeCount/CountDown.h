#pragma once

#include <Rendering/Sprite/Sprite.h>
#include <Systems/Audio/AudioSystem.h>


#include <vector>

class CountDown
{
public:
    CountDown() = default;
    ~CountDown() = default;

    void Initialize(float _gameTime, const std::vector<float>& _countDownTime);
    void Update();
    void Draw();

private:

    Vector2 position_ = { 640,360 };
    Vector2 size_ = { 100,100 };

    float currentTime_ = 0.0f;

    std::vector<Sprite> drawSprite_ = {};

    std::vector<float> countDownTime_ = {};
    std::vector<Sprite*> numberSprite_;

    uint32_t countHandle_ = 0;


    void ImGui();
};