#pragma once

#include <Rendering/Model/ObjectModel.h>

#include <Physics/Math/Vector4.h>
#include <memory>
#include <utility>

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

    float lifeTime_ = .4f;
    float currentTime_ = 0;

    std::unique_ptr<ObjectModel> ring_ = nullptr;

    float scale_ = 0;
    float sScale_ = 0;
    float eScale_ = 5;

    Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
    Vector4 sColor_ = { 1.0f,0.0f,0.0f,1.0f };
    Vector4 eColor_ = { 1.0f,0.0f,0.0f,0.0f };


    std::unique_ptr<ObjectModel> plane_ = nullptr;
    uint32_t textureHandle_ = 0;

    std::array<std::tuple<float, Vector2>, 3>keyFrame_;


};
