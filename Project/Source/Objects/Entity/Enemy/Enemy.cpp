#include "Enemy.h"

void Enemy::Initialize(Camera* camera)
{
	camera_ = camera;

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Cube/cube.obj", "Enemy");

	/*===============================================================//
					 　　	  コライダー設定
	//===============================================================*/

	collider_ = std::make_unique<Collider>();
	collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
	collider_->SetShape(oModel_->GetMin(), oModel_->GetMax());
	collider_->SetAtrribute("Enemy");
	collider_->SetMask({ "Enemy" });
	collider_->SetGetWorldMatrixFunc([this]() { return oModel_->GetWorldTransform()->matWorld_; });
	collider_->SetOnCollisionFunc([this](const Collider* other) { OnCollision(other); });
	collider_->SetReferencePoint({ 1.0f, 1.0f, 1.0f });

	// 敵オブジェクトをコライダーの所有者として設定
	collider_->SetOwner(this);
}

void Enemy::Update()
{
	collider_->RegsterCollider();
	oModel_->Update();
	// ImGui デバッグ表示
	if (ImGui::Begin("Enemy Debug")) {
		ImGui::Text("Enemy Attributes");
		ImGui::Text("Type: %s", GetCurrentTypeName().c_str());
		ImGui::Text("Alive: %s", isAlive_ ? "Yes" : "No");
	}
	ImGui::End();
}

void Enemy::Draw(const Vector4& color)
{
	//collider_->Draw();
	oModel_->Draw(camera_, color);
}

void Enemy::OnCollision(const Collider* other)
{

	// プレイヤーと当たったら
	if (other->GetName() == "Player") {

	}
	else if (other->GetName() == "SouthBullet"){

	
	}
	else if (other->GetName() == "NorthBullet") {


	}
}
