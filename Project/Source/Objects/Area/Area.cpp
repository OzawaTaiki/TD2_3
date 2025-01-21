#include "Area.h"

#include <Source/Objects/Entity/Player/Player.h>

void Area::Initialize(float _radius)
{
    model_ = std::make_unique<ObjectModel>();
    model_->Initialize("circle/circle.obj", "Area");

    model_->scale_ = { _radius,1,_radius };

}

void Area::Update(Player* _player)
{
#ifdef _DEBUG

    UVTransform& uvTransform = model_->GetUVTransform();

    Vector2 trans = uvTransform.GetOffset();
    Vector2 scale = uvTransform.GetScale();
    float rotation = uvTransform.GetRotation();

    ImGui::Begin("Area");
    ImGui::SeparatorText("UVTRans");
    ImGui::DragFloat2("scale", &scale.x, 0.01f);
    ImGui::DragFloat2("translate", &trans.x, 0.01f);
    ImGui::DragFloat("rotation", &rotation, 0.01f);

    ImGui::SeparatorText("Model");

    ImGui::DragFloat3("Translate", &model_->translate_.x, 0.1f);
    ImGui::DragFloat3("Scale", &model_->scale_.x, 0.1f);
    if (model_->useQuaternion_)
        ImGui::DragFloat4("Quaternion", &model_->quaternion_.x, 0.1f);
    else
        ImGui::DragFloat3("Rotate", &model_->rotate_.x, 0.1f);

    ImGui::Checkbox("useQuaternion", &model_->useQuaternion_);


    uvTransform.SetScale(scale);
    uvTransform.SetOffset(trans);
    uvTransform.SetRotation(rotation);
    ImGui::End();

#endif // _DEBUG

    ClampPlayerToCircle(_player);

    model_->Update();
}

void Area::Draw(const Camera* _camera)
{
    model_->Draw(_camera, { 1,1,1,1 });
}

void Area::ClampPlayerToCircle(Player* _player)
{
    Vector3 pos = _player->GetWorldPosition();
    Vector3 center = model_->GetWorldTransform()->transform_;
    float radius = model_->scale_.x;

    Vector3 direction = pos - center;
    float length = direction.Length();

    direction = direction.Normalize();


    // 半径よりも少し内側
    constexpr float innerMargin = 2.0f;

    // 有効半径 判定用の半径
    float keffectiveRadius = radius - innerMargin;

    // 半径よりも外側
    if (length > keffectiveRadius)
    {
        // 半径方向に移動
        pos = center + direction * keffectiveRadius;

        _player->SetTranslate(pos);
    }

}
