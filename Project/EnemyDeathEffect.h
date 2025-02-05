#pragma once

#include <Rendering/Model/ObjectModel.h>

#include <Physics/Math/Vector4.h>
#include <memory>

class Camera;
class EnemyDeathEffect
{
public:

    EnemyDeathEffect() = default;
    ~EnemyDeathEffect() = default;

    void Initialize(const Vector3& _pos);
    void Update();
    void Draw(Camera* _camera);

    bool IsEnd() const { return currentTime_ >= lifeTime_; }

private:

    float lifeTime_ = .5f;
    float currentTime_ = 0;

    std::unique_ptr<ObjectModel> oModel_ = nullptr;

    float scale_ = 0;
    float sScale_ = 0;
    float eScale_ = 3;

    Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
    Vector4 sColor_ = { 1.0f,1.0f,1.0f,1.0f };
    Vector4 eColor_ = { 1.0f,1.0f,1.0f,0.0f };
};
