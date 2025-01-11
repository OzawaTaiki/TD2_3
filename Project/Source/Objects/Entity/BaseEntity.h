#pragma once

#include <Rendering/Model/ObjectModel.h>
#include <Framework/Camera/Camera.h>
#include <Systems/Input/Input.h>
#include <memory>

class BaseEntity
{
public:
    virtual ~BaseEntity() = default;

    /// <summary>
    /// 初期化
    /// </summary>
    virtual void Initialize(Camera* camera) = 0;

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw(const Vector4& color) = 0;

protected:
    /// <summary>
    /// ワールド座標の取得
    /// </summary>
    Vector3 GetWorldPosition() const;

protected:
    Camera* camera_ = nullptr;
    std::unique_ptr<ObjectModel> oModel_ = nullptr;
};
