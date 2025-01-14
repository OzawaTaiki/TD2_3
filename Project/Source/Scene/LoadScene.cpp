#include "LoadScene.h"

#include <ResourceManagement/TextureManager/TextureManager.h>
#include <Systems/Utility/Debug.h>

std::unique_ptr<BaseScene> LoadScene::Create()
{
    Utils::Log("\n\nLoadScene::Create()\n");
    return std::make_unique<LoadScene>();
}

void LoadScene::Initialize()
{
    uint32_t  textureHandle = TextureManager::GetInstance()->Load("LoadTest.png");

    sprite_ = Sprite::Create(textureHandle);
    sprite_->Initialize();
    sprite_->SetSize({ 100, 100});
    sprite_->translate_ = { 640,360 };

    Utils::Log("LoadScene::Initialize()\n");
}

void LoadScene::Update()
{
    sprite_->rotate_ += 0.01f;

    sprite_->Update();
}

void LoadScene::Draw()
{
    Sprite::PreDraw();
    sprite_->Draw();
}
