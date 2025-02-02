#pragma once

#include <Rendering/Sprite/Sprite.h>
#include <Systems/Audio/AudioSystem.h>
#include <Systems/JsonBinder/JsonBinder.h>


#include <vector>
#include <cstdint>

class CountDown
{
public:
    CountDown() = default;
    ~CountDown() = default;

    void Initialize();
    void Update();
    void Draw();

private:

    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    Vector2 position_ = { 640,360 };
    Vector2 size_ = { 100,100 };
    Vector2 offset_ = { 0,0 };

    float currentTime_ = 0.0f;
    float gameTime_ = 0;

    std::vector<size_t> drawSprite_ = {};

    std::vector<float> countDownTime_ = {};
    std::vector<Sprite*> numberSprite_;

    uint32_t countHandle_ = 0;


    void ImGui();
};