#include "EnemyDeathEffect.h"

#include <Framework/Camera/Camera.h>
#include <Systems/Time/GameTime.h>
#include <Physics/Math/MyLib.h>

void EnemyDeathEffect::Initialize(const Vector3& _pos)
{
    oModel_ = std::make_unique<ObjectModel>();
    oModel_->Initialize("Ring/Ring.obj", "Ring");

    oModel_->translate_ = _pos;

}

void EnemyDeathEffect::Update()
{
    float deltaTime = GameTime::GetChannel("default").GetDeltaTime<float>();

    currentTime_ += deltaTime;

    float t = currentTime_ / lifeTime_;

    if (t > 1.0f)
        t = 1.0f;

    color_ = Lerp(sColor_, eColor_, t);
    scale_ = Lerp(sScale_, eScale_, t);

    oModel_->scale_ = { scale_,scale_,scale_ };

    oModel_->Update(false);
}

void EnemyDeathEffect::Draw(Camera* _camera)
{
    oModel_->Draw(_camera, color_);
}
