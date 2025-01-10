#include "Player.h"
#include <Physics/Math/VectorFunction.h>
#include <algorithm>


#include <numbers>
#include <windows.h>
#include <DirectXMath.h>
#include <Physics/Math/MatrixFunction.h>

using namespace DirectX;
void Player::Initialize(Camera* camera)
{
	camera_ = camera;
	input_ = Input::GetInstance();

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Debug/Debug.obj", "Player");

	jsonBinder_ = std::make_unique<JsonBinder>("PlayerData", "Resources/Data");

	// 回転軸の初期化
	rotation_ = { 0.0f,0.0f,0.0f };

	/*===============================================================//
						 　　  コライダー設定
	//===============================================================*/

	collider_ = std::make_unique<Collider>();
	collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
	collider_->SetShape(oModel_->GetMin(),oModel_->GetMax());
	collider_->SetAtrribute("Player");
	collider_->SetMask({ "Player" });
	collider_->SetGetWorldMatrixFunc([this]() { return oModel_->GetWorldTransform()->matWorld_; }); 
	collider_->SetOnCollisionFunc([this](const Collider* other) { OnCollision(other); }); 
	collider_->SetReferencePoint({ 0.0f, 0.0f, 0.0f }); 
}

void Player::Update()
{
	collider_->RegsterCollider();

	Move();

	Rotate();

	Bulletdelete();

	Fire();

	UpdateBullet();

	oModel_->Update();



#ifdef _DEBUG
	ImGui();
#endif // _DEBUG
}

void Player::Draw(const Vector4& color)
{
	//collider_->Draw();


	oModel_->Draw(camera_, color);



	for (NorthPoleBullet* bullet : bulletsNorth_) {
		bullet->Draw(*camera_,Vector4{255.0f,0.0f,0.0f,1.0f});
	}

	for (SouthPoleBullet* bullet : bulletsSouth_) {
		bullet->Draw(*camera_, Vector4{0.0f,0.0f,255.0f,1.0f});
	}
}

void Player::OnCollision(const Collider* other)
{

	if (other->GetName() == "Enemy") {

	}
}

void Player::Move()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0.0f, 0.0f, 0.0f };

#ifdef _DEBUG
	if (input_->IsKeyPressed(DIK_W)) {
		move.z += kCharacterSpeed_;
	}
	if (input_->IsKeyPressed(DIK_A)) {
		move.x -= kCharacterSpeed_;
	}
	if (input_->IsKeyPressed(DIK_S)) {
		move.z -= kCharacterSpeed_;
	}
	if (input_->IsKeyPressed(DIK_D)) {
		move.x += kCharacterSpeed_;
	}
#endif // _DEBUG

	if (input_->IsControllerConnected()) {
		XINPUT_STATE xInputState;
		ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
		if (XInputGetState(0, &xInputState) == ERROR_SUCCESS) {
			// 左スティックのX軸とY軸の値を取得（-32768 ～ 32767）
			float thumbLX = static_cast<float>(xInputState.Gamepad.sThumbLX);
			float thumbLY = static_cast<float>(xInputState.Gamepad.sThumbLY);
			if (fabs(thumbLX) > kDeadZoneL_) {
				move.x += (thumbLX / 32767.0f) * kCharacterSpeed_;
			}
			if (fabs(thumbLY) > kDeadZoneL_) {
				move.z += (thumbLY / 32767.0f) * kCharacterSpeed_;
			}
		}
	}


	// ワールド座標に移動を反映
	oModel_->translate_ += move;
}

void Player::Rotate()
{
	/*===============================================================//
						 　　  コントローラー
	//===============================================================*/
	if (input_->IsControllerConnected()) {
		XINPUT_STATE xInputState;
		ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
		if (XInputGetState(0, &xInputState) == ERROR_SUCCESS) {
			// 右スティックのX軸とY軸の値を取得（-32768 ～ 32767）
			float thumbRX = static_cast<float>(xInputState.Gamepad.sThumbRX);
			float thumbRY = static_cast<float>(xInputState.Gamepad.sThumbRY);

			// デッドゾーンをチェック
			if (fabs(thumbRX) > kDeadZoneR_ || fabs(thumbRY) > kDeadZoneR_) {
				// 目標角度を計算
				float newYaw = std::atan2(thumbRX, thumbRY);

				// 現在の角度との誤差を計算
				float diff = newYaw - rotation_.y;

				// -π～+π の範囲に収める（角度の折り返し対応）
				while (diff < -std::numbers::pi) diff += std::numbers::pi * 2.0f;
				while (diff > std::numbers::pi) diff -= std::numbers::pi * 2.0f;

				// ここで「角度差が一定以上の場合のみ更新」する
				const float kAngleThreshold = 0.001f;
				// 例: 0.015f は約 0.86°(度) に相当 (0.015 * 180 / π)

				if (fabs(diff) > kAngleThreshold) {
					// 角度差が大きい場合のみ更新するので、ガタガタしにくい
					rotation_.y = newYaw;
				}
			}
		}
	}

	/*===============================================================//
					 　　		マウス
	//===============================================================*/

	oModel_->rotate_ = rotation_;
}

void Player::Fire()
{
	NorthPoleBulletFire();

	SouthPoleBulletFire();
}

void Player::NorthPoleBulletFire()
{
	if (input_->IsPadTriggered(PadButton::iPad_RB) /*|| input_->IsMouseTriggered(0)*/) {

		// プレイヤーの向きから弾の初速度を計算
		float direction = rotation_.y + std::numbers::pi / 2.0f;
		Vector3 velocity(
			sin(direction) * bulletVelocity_, // X方向の速度
			0.0f,                               // Y方向の速度
			cos(direction) * bulletVelocity_  // Z方向の速度
		);

		Vector3 pos = GetWorldPosition();

		// 弾を生成し、初期化
		NorthPoleBullet* newBullet = new NorthPoleBullet();
		newBullet->Initialize("Sphere/sphere.obj", "North",pos,velocity);

		// 弾を登録する
		bulletsNorth_.push_back(newBullet);

	}

}

void Player::SouthPoleBulletFire()
{
	if (input_->IsPadTriggered(PadButton::iPad_LB) /*|| input_->IsMouseTriggered(1)*/) {
		float direction = rotation_.y + std::numbers::pi / 2.0f;
		// プレイヤーの向きから弾の初速度を計算
		Vector3 velocity(
			sin(direction) * bulletVelocity_, // X方向の速度
			0.0f,                               // Y方向の速度
			cos(direction) * bulletVelocity_  // Z方向の速度
		);

		velocity *= -1.0f;
		Vector3 pos = GetWorldPosition();

		// 弾を生成し、初期化
		SouthPoleBullet* newBullet = new SouthPoleBullet();
		newBullet->Initialize("Sphere/sphere.obj", "South",pos, velocity);

		// 弾を登録する
		bulletsSouth_.push_back(newBullet);

	}
}

void Player::UpdateBullet()
{
	// 弾の更新
	for (NorthPoleBullet* bullet : bulletsNorth_) {
		bullet->Update();
	}

	for (SouthPoleBullet* bullet : bulletsSouth_) {
		bullet->Update();
	}
}

void Player::Bulletdelete() {
	// デスフラグの立った弾を削除
	bulletsNorth_.remove_if([](NorthPoleBullet* bullet) {
		if (!bullet->IsAlive()) {
			delete bullet;
			return true;
		}
		return false;
		});

	bulletsSouth_.remove_if([](SouthPoleBullet* bullet) {
		if (!bullet->IsAlive()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void Player::Save()
{

}

void Player::ImGui()
{
	ImGui::Begin("Player");

	ImGui::DragFloat("L_Stick DeadZone", &kDeadZoneL_);
	ImGui::DragFloat("Speed", &kCharacterSpeed_,0.01f,1.0f);

	ImGui::End();
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = oModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldPos.y = oModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldPos.z = oModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldPos;
}