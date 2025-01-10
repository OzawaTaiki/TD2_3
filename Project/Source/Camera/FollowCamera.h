#pragma once

#include <Physics/Math/Vector3.h>
#include <Physics/Math/Matrix4x4.h>
#include <Rendering/Model/ObjectModel.h>

#include <memory>
#include <DirectXMath.h>
class FollowCamera
{
public:

    void Initialize();
    void Update();

    void FollowProcess();
    void SetTarget(const WorldTransform& target) { target_ = &target; }
    void GetViewProjection() const;

    Vector3 translate_ = { 0,100,0 };
    Vector3 scale_ = { 1,1,1 };
    Vector3 rotate_ = { DirectX::XMConvertToRadians(90.0f),0,0 };

    Matrix4x4 matView_ = {};

private:
    // 追従対象
    const WorldTransform* target_ = nullptr;
};

