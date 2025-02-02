#include "CountDown.h"

#include <Systems/Time/GameTime.h>

#include <ResourceManagement/TextureManager/TextureManager.h>

void CountDown::Initialize()
{

    countHandle_ = AudioSystem::GetInstance()->SoundLoadWave("count.wav");

    uint32_t textureHandle = TextureManager::GetInstance()->Load("countNumber.png", "Resources/Textures/");

    for (int i = 0; i < 10; ++i)
    {
        Sprite* sprite = Sprite::Create(textureHandle);
        sprite->Initialize();
        sprite->SetAnchor({ 0.5f,0.5f });
        sprite->uvScale_ = { 0.1f,1.0f };
        sprite->uvTranslate_ = { i * 0.1f, 0.0f };
        sprite->SetColor({ 1.0f,1.0f,1.0f,0.3f });
        numberSprite_.push_back(sprite);
    }

    jsonBinder_ = std::make_unique<JsonBinder>("countDown", "Resources/Data/Parameter/");
    jsonBinder_->RegisterVariable("gameTime", &gameTime_);
    jsonBinder_->RegisterVariable("countTime", &countDownTime_);
    jsonBinder_->RegisterVariable("position", &position_);
    jsonBinder_->RegisterVariable("size", &size_);
    jsonBinder_->RegisterVariable("offset", &offset_);

    if (gameTime_ == 0.0f)
        gameTime_ = 60.0f;
    if (countDownTime_.empty())
        countDownTime_ = { 30.0f,10.0f,5.0f,4.0f,3.0f,2.0f,1.0f,0.0f };

    currentTime_ = gameTime_;

}

void CountDown::Update()
{
    float deltaTime = GameTime::GetInstance()->GetUnScaleDeltaTime_float();

    bool isCount = false;
    float preTime = std::floorf(currentTime_);

    currentTime_ -= deltaTime;


    float curTime = std::ceilf(currentTime_);

    for (float time : countDownTime_)
    {
        if (curTime == time + 1 && curTime == preTime)
        {
            isCount = true;
        }
        else if (!drawSprite_.empty() && curTime == preTime)
        {
            drawSprite_.clear();

        }
    }

    if (isCount)
    {
        AudioSystem::GetInstance()->SoundPlay(countHandle_, 1.0f, false, true);

        drawSprite_.clear();

        float ten = currentTime_ / 10.0f;
        float one = std::fmodf(currentTime_, 10.0f);

        size_t t = static_cast<size_t>(std::floorf(ten));
        size_t o = static_cast<size_t>(std::floorf(one));

        drawSprite_.push_back(t);
        drawSprite_.push_back(o);
    }



    ImGui();

}

void CountDown::Draw()
{
    for (size_t i = 0; i < drawSprite_.size(); ++i)
    {
        auto& sprite = numberSprite_[i];

        sprite->SetSize(size_);
        sprite->translate_ = { position_.x + offset_.x * i,position_.y };
        sprite->uvTranslate_ = { static_cast<float>(drawSprite_[i]) * 0.1f,0.0f };

        sprite->Draw();
    }

}

void CountDown::ImGui()
{
#ifdef _DEBUG

    ImGui::Begin("CountDown");

    ImGui::Text("currentTime:%f", currentTime_);
    ImGui::DragFloat2("position", &position_.x);
    ImGui::DragFloat2("size", &size_.x);
    ImGui::DragFloat("offset", &offset_.x);

    ImGui::InputFloat("gameTime", &gameTime_);

    if(ImGui::Button("save"))
        jsonBinder_->Save();


    ImGui::End();

#endif // _DEBUG

}
