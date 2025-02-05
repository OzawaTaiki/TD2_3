#include "LoadScene.h"
#include <ResourceManagement/TextureManager/TextureManager.h>
#include <Systems/Utility/Debug.h>
#include <Systems/Time/GameTime.h>
#include <random>

const std::string defaulFilPath = "Resources./Textures./";

std::unique_ptr<BaseScene> LoadScene::Create()
{
    return std::make_unique<LoadScene>();
}

void LoadScene::Initialize()
{
    // テクスチャの読み込み
    textureHandles_[0] = TextureManager::GetInstance()->Load("load1.png", defaulFilPath);
    textureHandles_[1] = TextureManager::GetInstance()->Load("load2.png", defaulFilPath);
    textureHandles_[2] = TextureManager::GetInstance()->Load("load3.png", defaulFilPath);

    // スプライトの初期設定
    sprite_ = Sprite::Create(textureHandles_[0]);
    sprite_->Initialize();
    sprite_->SetSize({ 100, 100 });
    sprite_->translate_ = { 640, 360 };

    Utils::Log("LoadScene::Initialize()\n");
}

void LoadScene::Update()
{
    // デルタタイムを取得
    float deltaTime = GameTime::GetInstance()->GetChannel("default").GetDeltaTime<float>();

    // 静的な乱数ジェネレータを準備
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.5);

    // 回転
    //sprite_->rotate_ += rotationSpeed_;

    // テクスチャ切り替え
    elapsedTime_ += deltaTime;
    if (elapsedTime_ >= textureChangeInterval_)
    {
        currentTextureIndex_ = (currentTextureIndex_ + 1) % 3;
        sprite_->SetTextureHandle(textureHandles_[currentTextureIndex_]);

        // 次の切り替え間隔をランダムに設定
        textureChangeInterval_ = dis(gen);

        // 経過時間をリセット
        elapsedTime_ = 0.0f;
    }

    // スケールアニメーション
    if (isScalingUp_)
    {
        scale_ += 0.02f;
        if (scale_ >= 1.2f) isScalingUp_ = false;
    }
    else
    {
        scale_ -= 0.02f;
        if (scale_ <= 0.8f) isScalingUp_ = true;
    }

    // スプライトのスケールを更新
    sprite_->scale_ = { 1, 1};

    sprite_->Update();
}

void LoadScene::Draw()
{
    Sprite::PreDraw();
    sprite_->Draw();
}