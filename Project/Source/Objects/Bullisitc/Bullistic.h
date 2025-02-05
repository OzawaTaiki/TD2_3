#pragma once

#include <Rendering/Model/ObjectModel.h>

#include <Systems/JsonBinder/JsonBinder.h>

#include <cstdint>

class Camera;
class Bullistic
{
public:
    Bullistic() = default;
    ~Bullistic() = default;

    void Initialize(const WorldTransform* _parent, const std::string& _name = "right");
    void Update();
    void Draw(Camera* _camera);

private:

    void ImGui();
    void InitBullistics();
    void InitJson(const std::string& _name);

    size_t modelNum_ = 0;   // 描画するモデルの数
    static constexpr size_t MAX_MODEL_NUM = 32;    // 最大描画モデル数

    float range_ = 0.0f;
    Vector3 scale_ = {};

    Vector3 offset_ = {};
    Vector4 color_ = {};

    std::vector<std::unique_ptr<ObjectModel>> ballistics_;
    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

    std::array< bool, 10> isDraw_ = {};

    const WorldTransform* parent_ = nullptr;


};