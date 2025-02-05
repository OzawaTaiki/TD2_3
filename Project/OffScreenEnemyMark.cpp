#include "OffScreenEnemyMark.h"
#include <Physics/Math/MyLib.h>
#include <ResourceManagement/TextureManager/TextureManager.h>
#include <Core/WinApp/WinApp.h>
#include <Framework/Camera/Camera.h>

const Vector2 OffScreenEnemyMark::leftTop_ = { 0,0 };
const Vector2 OffScreenEnemyMark::rightTop_ = { 1280,0 };
const Vector2 OffScreenEnemyMark::leftBottom_ = { 0,720 };
const Vector2 OffScreenEnemyMark::rightBottom_ = { 1280,720 };
const Vector2 OffScreenEnemyMark::center_ = { 640,360 };


void OffScreenEnemyMark::Initialize()
{
    sprite_ = Sprite::Create(TextureManager::GetInstance()->Load("enemyDirection.png", "Resources/Textures/"));
    sprite_->Initialize();
    sprite_->SetAnchor({ 0.5f,0.5f });
    sprite_->SetSize({ 100,100 });
}

void OffScreenEnemyMark::Update(const Camera& _camer, const Vector3& _pos)
{
    isDraw_ = true;

    // ２次元座標に変換
    Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, WinApp::kWindowWidth_, WinApp::kWindowHeight_, 0, 1);

    Matrix4x4 matViewProjectionViewport = _camer.matView_ * _camer.matProjection_ * viewportMatrix;

    Vector3 pos = Transform(_pos, matViewProjectionViewport);

    Vector2 screenPos = { pos.x,pos.y };

    Vector2 crossPos = {};

    crossPos = getCrossPos(leftTop_, rightTop_, screenPos, center_);
    if (crossPos != Vector2{ 0,0 })
    {
        sprite_->translate_ = crossPos + Vector2(0, 32);
        return;
    }

    crossPos = getCrossPos(rightTop_, rightBottom_, screenPos, center_);
    if (crossPos != Vector2{ 0,0 })
    {
        sprite_->translate_ = crossPos + Vector2(-32, 0);
        return;
    }

    crossPos = getCrossPos(rightBottom_, leftBottom_, screenPos, center_);
    if (crossPos != Vector2{ 0,0 })
    {
        sprite_->translate_ = crossPos + Vector2(0, -32);
        return;
    }

    crossPos = getCrossPos(leftBottom_, leftTop_, screenPos, center_);
    if (crossPos != Vector2{ 0,0 })
    {
        sprite_->translate_ = crossPos + Vector2(32, 0);
        return;
    }

    isDraw_ = false;


}

void OffScreenEnemyMark::Draw()
{
    if (!isDraw_) return;

    sprite_->Update();
    sprite_->Draw();
}
