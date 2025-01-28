#include "SouthPoleBullet.h"
#include "../Entity/Enemy/Enemy.h"
#include "../../System_TD/TypeManager/TypeManager.h"

void SouthPoleBullet::Initialize(const std::string& directoryPath, const std::string& name, const Vector3& translate, const Vector3& velocity, const Vector3& acceleration)
{
	BaseBullet::Initialize(directoryPath, name, translate, velocity,acceleration);

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


	// 弾オブジェクトをコライダーの所有者として設定
	collider_->SetOwner(this);
}

void SouthPoleBullet::Update()
{
	if (!isAlive_) return; // 生存状態を確認

	BaseBullet::Update();
	collider_->RegsterCollider();
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
		// コライダーの所有者から敵オブジェクトを取得
		Enemy* enemy = static_cast<Enemy*>(other->GetOwner());
		if (enemy) {
			TypeManager::ApplyTypeChange(enemy, TypeManager::BulletType::South);
		}
		isAlive_ = false; // 弾を削除
	}
}
