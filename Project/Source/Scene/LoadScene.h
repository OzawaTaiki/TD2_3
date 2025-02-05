#pragma once

#include <Framework/eScene/BaseScene.h>

#include <Rendering/Sprite/Sprite.h>

class LoadScene : public BaseScene
{
public:
    LoadScene() = default;
    ~LoadScene() = default;

    static std::unique_ptr<BaseScene> Create();

    void Initialize() override;
    void Update() override;
    void Draw() override;


private:
    /*===============================================================//
                         　　 ポインタなど
    //===============================================================*/
    Sprite* sprite_;
    uint32_t textureHandles_[3];
    int currentTextureIndex_ = 0;
    float rotationSpeed_ = 0.05f;
    float textureChangeInterval_ = 0.2f; // テクスチャ切り替え間隔
    float elapsedTime_ = 0.0f;
    float scale_ = 1.0f;
    bool isScalingUp_ = true;

};