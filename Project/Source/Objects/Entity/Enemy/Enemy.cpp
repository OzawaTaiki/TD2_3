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
	oModel_->Update();
	// ImGui デバッグ表示
	if (ImGui::Begin("Enemy Type")) {
		ImGui::Text("Type: %s", GetCurrentTypeName().c_str());
		ImGui::Text("Alive: %s", isAlive_ ? "Yes" : "No");
	}
	ImGui::End();
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

Vector3 Enemy::GetCenterPosition() const {
	// 見た目上の中心点のオフセット（モデル座標系）
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標系に変換
	Vector3 worldPos = Transform(offset, oModel_->GetWorldTransform()->matWorld_);
	return worldPos;
}
