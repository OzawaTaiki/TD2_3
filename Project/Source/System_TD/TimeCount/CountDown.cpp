#include "CountDown.h"

#include <Systems/Time/GameTime.h>

#include <ResourceManagement/TextureManager/TextureManager.h>

void CountDown::Initialize(float _gameTime, const std::vector<float>& _countDownTime)
{
    currentTime_ = _gameTime;

    countDownTime_ = _countDownTime;

    countHandle_ = AudioSystem::GetInstance()->SoundLoadWave("count.wav");

    uint32_t textureHandle = TextureManager::GetInstance()->Load("countNumber.png", "Resources/Textures/");

    for (int i = 0; i < 10; ++i)
    {
        Sprite* sprite = Sprite::Create(textureHandle);
        sprite->Initialize();
        sprite->SetAnchor({ 0.5f,0.5f });
        sprite->uvScale_ = { 0.1f,1.0f };
        sprite->uvTranslate_ = { i * 0.1f, 0.0f };
        numberSprite_.push_back(sprite);
    }
}

void CountDown::Update()
{
    float deltaTime = GameTime::GetInstance()->GetUnScaleDeltaTime_float();

    bool isCount = false;
    float preTime = std::ceilf(currentTime_);

    currentTime_ -= deltaTime;


    float curTime = std::floorf(currentTime_);

    for (float time : countDownTime_)
    {
        if (currentTime_ == time && curTime == preTime)
        {
            isCount = true;
        }
    }

    if (isCount)
    {
        AudioSystem::GetInstance()->SoundPlay(countHandle_, 1.0f, false, true);
    }

    drawSprite_.clear();

    float ten = currentTime_ / 10.0f;
    float one = std::fmodf(currentTime_, 10.0f);

    size_t t = static_cast<size_t>(std::floorf(ten));
    size_t o = static_cast<size_t>(std::floorf(one));


    drawSprite_.push_back(*numberSprite_[t]);
    drawSprite_.push_back(*numberSprite_[o]);

    for (size_t i = 0; i < drawSprite_.size(); ++i)
    {
        drawSprite_[i].SetAnchor({ 0.5f,0.5f });
        drawSprite_[i].SetSize(size_);
        drawSprite_[i].translate_ = { position_.x + (i * size_.x),position_.y };
    }

    ImGui();

}

void CountDown::Draw()
{
    for (auto& sprite : drawSprite_)
    {
        sprite.Draw();
    }
}

void CountDown::ImGui()
{
#ifdef _DEBUG

    ImGui::Begin("CountDown");

    ImGui::DragFloat2("position", &position_.x);
    ImGui::DragFloat2("size", &size_.x);

    ImGui::End();

#endif // _DEBUG

}
