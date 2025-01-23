#include "Enemy.h"
#include <Physics/Math/VectorFunction.h>

void Enemy::Initialize(Camera* camera)
{
	camera_ = camera;

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Sphere/sphere.obj", "Enemy");

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
	//collider_->SetReferencePoint({ 0.0f, 0.0f, 0.0f });

	// 敵オブジェクトをコライダーの所有者として設定
	collider_->SetOwner(this);

    markForRemoval_ = false;
}

void Enemy::Update()
{

	if (!isAlive_) return;

	ImGui();
	oModel_->Update();
	collider_->RegsterCollider();
}

void Enemy::Draw(const Vector4& color)
{
	//collider_->Draw();
	Vector4 typeColor;

	// 弾のタイプに応じた色を設定
	if (currentType_ == BulletType::None) {
		typeColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白
	}
	else if (currentType_ == BulletType::North) {
		typeColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤
	}
	else if (currentType_ == BulletType::South) {
		typeColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f); // 青
	}
	else {
		typeColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f); // 黒
	}

	collider_->Draw();
	oModel_->Draw(camera_, typeColor); // タイプごとの色で描画

}

void Enemy::Move(float& deltaTime)
{
	if (!isAlive_) return;
	// 速度に基づいて位置を更新
	oModel_->translate_ += velocity_ * deltaTime;

	// 目的地に到達したら生存フラグを下げる
	if ((oModel_->translate_ - goal_).Length() < 0.1f) {
		isAlive_ = false;
	}

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

void Enemy::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Enemy Para");
	ImGui::DragFloat3("Velocity", &velocity_.x);


	ImGui::End();

#endif // _DEBUG


}

Vector3 Enemy::GetCenterPosition() const {
	// 見た目上の中心点のオフセット（モデル座標系）
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標系に変換
	Vector3 worldPos = Transform(offset, oModel_->GetWorldTransform()->matWorld_);
	return worldPos;
}
