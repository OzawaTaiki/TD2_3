#include "Area.h"

#include <Source/Objects/Entity/Player/Player.h>
#include <ResourceManagement/TextureManager/TextureManager.h>

void Area::Initialize(float _size)
{
    model_ = std::make_unique<ObjectModel>();
    model_->Initialize("plane/plane.gltf", "Area");

    model_->rotate_.x = std::numbers::pi_v<float> / 2;
    model_->scale_ = { _size * 2,_size * 2,1 };


    circleModel_ = std::make_unique<ObjectModel>();
    circleModel_->Initialize("circle/circle.obj", "Circle");
    circleTextureHandle_ = TextureManager::GetInstance()->Load("Tile.png", "Resources/models/tile/");

    circleModel_->scale_ = { _size ,1 ,_size };
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
    circleModel_->Update();
}

void Area::Draw(const Camera* _camera)
{
    model_->Draw(_camera, textureHandle_, { 0.5f,0.5f,0.5f,1 });
    circleModel_->Draw(_camera, circleTextureHandle_, { 0.5f,0.5f,0.5f,1 });
}

void Area::ClampPlayerToCircle(Player* _player)
{
    Vector3 pos = _player->GetWorldPosition();
    Vector3 center = model_->GetWorldTransform()->transform_;
    float radius = model_->scale_.x / 2;

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
