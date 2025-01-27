#include "Enemy.h"
#include "../Player/Player.h"
#include "../../../System_TD/ScoreManager/ScoreManager.h"
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
	isAddScore_ = false;
}

void Enemy::Update()
{
	//isAddScore_ = false;

	// 敵が生存していない場合は処理を終了

	// スコアをまだ加算していない場合のみ加算

	if (!isAlive_) {
		ScoreManager::GetInstance()->AddScore(100);
	}



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


	if (isDraw_) {
		collider_->Draw();
		oModel_->Draw(camera_, typeColor); // タイプごとの色で描画
	}

}

void Enemy::ChangeType(float deltaTime)
{
	if (currentType_ != BulletType::None) {
		typeChangeCount_ += deltaTime;
		if (typeChangeCount_ >= typeChangeTime_) {
			SetCurrentType(BulletType::None);  // None状態に変更
			typeChangeCount_ = 0.0f;   // タイマーをリセット
		}
	}
	else {
		typeChangeCount_ = 0.0f;  // Noneのときはタイマーをリセット
	}

}

void Enemy::Move(float& deltaTime)
{
	if (!isAlive_) return;

	// ゴールまでの位置ベクトルを求める
	// ベクトルの長さを求める
	// 正規化
	if (moveType_ == MoveType::Direct) {
		Vector3 direction = goal_ - oModel_->translate_;
		float distance = direction.Length();
		if (distance > 0.0f) {
			direction /= distance;
			oModel_->translate_ += direction * speed_ * deltaTime;
		}
	}
	else if (moveType_ == MoveType::Target && player_) {
		Vector3 direction = (player_->GetWorldPosition() - oModel_->translate_).Normalize();
		oModel_->translate_ += direction * speed_ * deltaTime;
	}



	// 目的地に到達したら生存フラグを下げる
	if ((oModel_->translate_ - goal_).Length() < 0.1f) {
		//isAlive_ = false;
		isDraw_ = false;
	}

}

void Enemy::OnCollision(const Collider* other)
{

	// プレイヤーと当たったら
	if (other->GetName() == "Player") {

	}
	else if (other->GetName() == "SouthBullet") {


	}
	else if (other->GetName() == "NorthBullet") {


	}
}

void Enemy::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Enemy Para");

	ImGui::Checkbox("AddScore", &isAddScore_);
	ImGui::Checkbox("Alive", &isAlive_);

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
