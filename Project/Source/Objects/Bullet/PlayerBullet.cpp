#include "PlayerBullet.h"

void PlayerBullet::Initialize(const Vector3& translate, const Vector3& velocity)
{
	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Sphere/Sphere.obj", "Player Bullet");

	//----------------------------------------------------//
	/*					メンバ変数に保存					　*/
	//----------------------------------------------------//
	oModel_->translate_ = translate;
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	if (--deathTimer_ <= 0) {
		isAlive_ = false;
	}
	oModel_->translate_ += velocity_;
	oModel_->Update();
}

void PlayerBullet::Draw(const Camera& camera, const Vector4& color)
{
	oModel_->Draw(&camera, color);
}

Vector3 PlayerBullet::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = oModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldPos.y = oModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldPos.z = oModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldPos;
}
