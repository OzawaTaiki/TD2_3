#include "Player.h"
#include <Physics/Math/VectorFunction.h>
#include <algorithm>


#include <numbers>
#include <windows.h>
#include <DirectXMath.h>
#include <Physics/Math/MatrixFunction.h>
#include <Systems/JsonBinder/JsonHub.h>

using namespace DirectX;
void Player::Initialize(Camera* camera)
{
	camera_ = camera;
	input_ = Input::GetInstance();

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Debug/Debug.obj", "Player");


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

	Knockback();

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
	// 無敵状態中なら衝突判定を無視する
	if (isKnockbackActive_) {
		return;
	}

	if (other->GetName() == "Enemy") {

		hp_--;
        if (hp_ <= 0) {
            isAlive_ = false;
        }
		// プレイヤーの向いている方向（rotation_.y）から前方ベクトルを算出
		float angle = rotation_.y;
		Vector3 forward{ sinf(angle), 0.0f, cosf(angle) };

		// 反対方向へノックバックさせる初期速度を与える
		knockbackVelocity_ = forward * (-knockbackStrength_);
		// ノックバック中にする＆無敵状態の時間を設定
		isKnockbackActive_ = true;
		knockbackInvincibleTime_ = knockbackInvincibleDuration_;

		if (isKnockbackActive_) {
			oModel_->translate_ = prePosition_;
		}

	}
}

void Player::Move()
{
	prePosition_ = oModel_->translate_;
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
	if (input_->IsControllerConnected()) {
		XINPUT_STATE xInputState;
		ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
		if (XInputGetState(0, &xInputState) == ERROR_SUCCESS) {
			// 左スティックのX軸とY軸の値を取得（-32768 ～ 32767）
			float thumbLX = static_cast<float>(xInputState.Gamepad.sThumbLX);
			float thumbLY = static_cast<float>(xInputState.Gamepad.sThumbLY);

			// デッドゾーンをチェック
			if (fabs(thumbLX) > kDeadZoneL_ || fabs(thumbLY) > kDeadZoneL_) {
				// 目標角度を計算
				float newYaw = std::atan2(thumbLX, thumbLY);

				// 現在の角度との誤差を計算
				float diff = newYaw - rotation_.y;

				// -π～+π の範囲に収める（角度の折り返し対応）
				while (diff < -std::numbers::pi) diff += std::numbers::pi * 2.0f;
				while (diff > std::numbers::pi) diff -= std::numbers::pi * 2.0f;

				// 角度差が一定以上の場合のみ更新
				const float kAngleThreshold = 0.001f;
				if (fabs(diff) > kAngleThreshold) {
					rotation_.y = newYaw;
				}
			}
		}
	}

	oModel_->rotate_ = rotation_;
}

void Player::Fire()
{
	NorthPoleBulletFire();

	SouthPoleBulletFire();
}

void Player::Knockback()
{
	if (knockbackVelocity_.Length() > 0.01f) {
		oModel_->translate_ += knockbackVelocity_;


		knockbackVelocity_ *= knockbackDamping_;
	}
	else {
		// 小さすぎる場合は0にリセット
		knockbackVelocity_ = { 0.0f, 0.0f, 0.0f };
	}

	// 無敵状態中なら、経過時間分だけ invincibleTime_ を減らす
	if (isKnockbackActive_) {
		// Δt は1フレームあたりの経過時間（もしフレームレート依存なら調整）
		const float deltaTime = 1.0f / 60.0f; // 例として60FPSの場合
		knockbackInvincibleTime_ -= deltaTime;
		if (knockbackInvincibleTime_ <= 0.0f) {
			isKnockbackActive_ = false;
		}
	}

}

void Player::NorthPoleBulletFire()
{
	if (input_->IsPadTriggered(PadButton::iPad_RB) /*|| input_->IsMouseTriggered(0)*/) {

		// プレイヤーの向きから弾の初速度を計算
		float direction = rotation_.y + std::numbers::pi_v<float> / 2.0f;
		Vector3 velocity(
			sinf(direction) * bulletVelocity_,	// X方向の速度
			0.0f,                               // Y方向の速度
			cosf(direction) * bulletVelocity_	// Z方向の速度
		);

		Vector3 acceleration(
			sinf(direction) * bulletAcceleration_,	// X方向の速度
			0.0f,									// Y方向の速度
			cosf(direction) * bulletAcceleration_	// Z方向の速度
		);

		Vector3 pos = GetWorldPosition();

		// 弾を生成し、初期化
		NorthPoleBullet* newBullet = new NorthPoleBullet();
		newBullet->Initialize("Sphere/sphere.obj", "North",pos,velocity,acceleration);

		// 弾を登録する
		bulletsNorth_.push_back(newBullet);

	}

}

void Player::SouthPoleBulletFire()
{
	if (input_->IsPadTriggered(PadButton::iPad_LB) /*|| input_->IsMouseTriggered(1)*/) {
		// プレイヤーの向きから弾の初速度を計算
		float direction = rotation_.y + std::numbers::pi_v<float> / 2.0f;
		Vector3 velocity(
			sinf(direction) * bulletVelocity_,	// X方向の速度
			0.0f,                               // Y方向の速度
			cosf(direction) * bulletVelocity_	// Z方向の速度
		);

		Vector3 acceleration(
			sinf(direction) * bulletAcceleration_,	// X方向の速度
			0.0f,									// Y方向の速度
			cosf(direction) * bulletAcceleration_	// Z方向の速度
		);

		velocity *= -1.0f;
		acceleration *= -1.0f;
		Vector3 pos = GetWorldPosition();

		// 弾を生成し、初期化
		SouthPoleBullet* newBullet = new SouthPoleBullet();
		newBullet->Initialize("Sphere/sphere.obj", "South",pos, velocity,acceleration);

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
	jsonBinder_->Save();
}

#ifdef _DEBUG
void Player::ImGui()
{
	ImGui::Begin("Player");

	// 入力系
	ImGui::DragFloat("L_Stick DeadZone", &kDeadZoneL_, 100.0f, 0.0f, 50000.0f, "%.0f");
	ImGui::DragFloat("Character Speed", &kCharacterSpeed_, 0.01f, 0.01f, 5.0f);

	// HP 関連
	ImGui::InputFloat("MaxHP", &maxHp_, 1.0f);
	if (ImGui::InputFloat("HP", &hp_, 1.0f))
	{
		if (hp_ > maxHp_) {
			hp_ = maxHp_;
		}
	}

	// 弾関連
	ImGui::Text("Bullet Info");
	ImGui::DragFloat("Bullet Velocity", &bulletVelocity_, 0.001f, 0.001f, 1.0f);
	ImGui::DragFloat("Bullet Acceleration", &bulletAcceleration_, 0.001f, 0.001f, 1.0f);

	// ノックバック関連
	ImGui::Separator();
	ImGui::Text("Knockback Settings");
	ImGui::DragFloat("Knockback Strength", (float*)&knockbackStrength_, 0.1f, 0.1f, 10.0f);
	ImGui::DragFloat("Knockback Damping", (float*)&knockbackDamping_, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Invincible Duration", (float*)&knockbackInvincibleDuration_, 0.1f, 0.0f, 5.0f);

	// ※ 必要ならノックバックの現在の状態なども表示
	ImGui::Text("Knockback Active: %s", isKnockbackActive_ ? "Yes" : "No");
	ImGui::Text("Knockback Invincible Time: %.2f", knockbackInvincibleTime_);

	// デバッグ用にプレイヤーの位置なども表示
	Vector3 pos = GetWorldPosition();
	ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);

	//if (ImGui::Button("Save Settings"))
	//{
	//	Save();
	//}

	ImGui::End();
}
#endif


Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = oModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldPos.y = oModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldPos.z = oModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetCenterPosition()
{
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset,oModel_->GetWorldTransform()->matWorld_);

	return worldPos;
}

void Player::InitJsonBinder()
{
    //JsonHub::GetInstance()->SetDirectoryPathFromRoot("Resources/Data/Parameter/");

	jsonBinder_ = std::make_unique<JsonBinder>("PlayerData", "Resources/Data/Parameter");

	jsonBinder_->RegisterVariable("MaxHP", &maxHp_);
	jsonBinder_->RegisterVariable("characterSpeed", &kCharacterSpeed_);
}
