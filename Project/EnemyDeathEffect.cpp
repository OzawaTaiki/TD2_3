#include "EnemyDeathEffect.h"

#include <Framework/Camera/Camera.h>
#include <Systems/Time/GameTime.h>
#include <Physics/Math/MyLib.h>
#include <ResourceManagement/TextureManager/TextureManager.h>

void EnemyDeathEffect::Initialize(const Vector3& _pos)
{
    ring_ = std::make_unique<ObjectModel>();
    ring_->Initialize("Ring/Ring.obj", "Ring");



    ring_->translate_ = _pos;

    plane_ = std::make_unique<ObjectModel>();
    plane_->Initialize("plane/plane_up.gltf", "Plane");
    plane_->translate_ = _pos;

    textureHandle_ = TextureManager::GetInstance()->Load("killEffect.png", "Resources/Textures/");

    // scale,rotate
    keyFrame_[0] = std::make_tuple(0.0f, Vector2(0.2f, 0));
    keyFrame_[1] = std::make_tuple(0.1f, Vector2(0.0f, -0.01f));
    keyFrame_[2] = std::make_tuple(1.0f, Vector2(10.0f, 1.57f * 2));

}

void EnemyDeathEffect::Update()
{
    float deltaTime = GameTime::GetChannel("default").GetDeltaTime<float>();

    currentTime_ += deltaTime;

    float t = currentTime_ / lifeTime_;

    if (t > 1.0f)
        t = 1.0f;

    for (int i = 0; i < keyFrame_.size() - 1; ++i)
    {
        if (std::get<0>(keyFrame_[i]) <= t && t < std::get<0>(keyFrame_[i + 1]))
        {
            float t0 = std::get<0>(keyFrame_[i]);
            float t1 = std::get<0>(keyFrame_[i + 1]);
            float tt = (t - t0) / (t1 - t0);
            Vector2 s0 = std::get<1>(keyFrame_[i]);
            Vector2 s1 = std::get<1>(keyFrame_[i + 1]);
            Vector2 s = Lerp(s0, s1, tt);
            plane_->scale_ = { s.x,s.x,s.x };
            plane_->rotate_.y = s.y;
        }
    }

    color_ = Lerp(sColor_, eColor_, t);
    scale_ = Lerp(sScale_, eScale_, t);

    ring_->scale_ = { scale_,scale_,scale_ };

    ring_->Update(false);
    plane_->Update(false);
}

void EnemyDeathEffect::Draw(Camera* _camera)
{
    ring_->Draw(_camera, color_);
    plane_->Draw(_camera, textureHandle_, {1,1,1,color_.w});
}
