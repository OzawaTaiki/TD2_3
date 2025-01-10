#include "SouthPoleBullet.h"

void SouthPoleBullet::Initialize(const std::string& directoryPath, const std::string& name, const Vector3& translate, const Vector3& velocity)
{
	BaseBullet::Initialize(directoryPath, name, translate, velocity);

	/*===============================================================//
					 　　  コライダー設定
	//===============================================================*/

	collider_ = std::make_unique<Collider>();
	collider_->SetBoundingBox(Collider::BoundingBox::Sphere_3D);
	collider_->SetShape(1.0f);
	collider_->SetAtrribute("SouthBullet");
	collider_->SetMask({ "Player" });
	collider_->SetGetWorldMatrixFunc([this]() { return oModel_->GetWorldTransform()->matWorld_; });
	collider_->SetOnCollisionFunc([this](const Collider* other) { OnCollision(other); });
	collider_->SetReferencePoint({ 0.0f, 0.0f, 0.0f });
}

void SouthPoleBullet::Update()
{
	if (!isAlive_) return; // 生存状態を確認
	collider_->RegsterCollider();
	BaseBullet::Update();
}

void SouthPoleBullet::Draw(const Camera& camera, const Vector4& color)
{
	if (!isAlive_) return; // 生存状態を確認
	//collider_->Draw();
	BaseBullet::Draw(camera, color);
}

void SouthPoleBullet::OnCollision(const Collider* other)
{
	if (other->GetName() == "Enemy") {
		isAlive_ = false;
	}
}
