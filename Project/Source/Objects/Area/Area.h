#pragma once

#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/WorldTransform.h>
#include <Rendering/Model/ObjectColor.h>
#include <Systems/UVTransform/UVTransform.h>

#include <string>

class Player;
class Camera;
class Area
{
public:
    Area() {};
    ~Area() = default;

    void Initialize(float _radius);
    void Update(Player* _player);
    void Draw(const Camera* _camera);

    void ClampPlayerToCircle(Player* _player);

private:

    std::unique_ptr<ObjectModel>model_ = nullptr;
    std::unique_ptr<ObjectModel>circleModel_ = nullptr;
    std::string name_ = "";
    uint32_t textureHandle_ = 0;
    uint32_t circleTextureHandle_ = 0;




};