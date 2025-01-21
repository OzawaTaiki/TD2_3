#include "BaseBullet.h"

void BaseBullet::Initialize(const std::string& directoryPath, const std::string& name, 
	const Vector3& translate, const Vector3& velocity, const Vector3& acceleration)
{
	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize(directoryPath, name);

	//----------------------------------------------------//
	/*					メンバ変数に保存					　*/
	//----------------------------------------------------//
	oModel_->translate_ = translate;
	velocity_ = velocity;
	acceleration_ = acceleration;
}

void BaseBullet::Update()
{
	if (--deathTimer_ <= 0) {
		isAlive_ = false;
	}
	velocity_ += acceleration_;
	oModel_->translate_ += velocity_;
	oModel_->Update();
}

void BaseBullet::Draw(const Camera& camera, const Vector4& color)
{
	oModel_->Draw(&camera, color);
}

Vector3 BaseBullet::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = oModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldPos.y = oModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldPos.z = oModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldPos;
}
