#include "Player.h"
#include <Physics/Math/VectorFunction.h>
#include <algorithm>


#include <numbers>
#include <windows.h>
#include <DirectXMath.h>
#include <Physics/Math/MatrixFunction.h>
#include <Systems/JsonBinder/JsonHub.h>
#include <Systems/Utility/RandomGenerator.h>
#include <Physics/Math/Easing.h>
#include <Physics/Math/MyLib.h>
#include <Source/System_TD/ComboManager/ComboManager.h>
#include <ResourceManagement/TextureManager/TextureManager.h>

const std::string defaulFilPath = "Resources./Textures./";
using namespace DirectX;
Player::~Player()
{
	// 弾削除
	for (NorthPoleBullet* bullet : bulletsNorth_) {
		delete bullet;
	}
	bulletsNorth_.clear();

	for (SouthPoleBullet* bullet : bulletsSouth_) {
		delete bullet;
	}
	bulletsSouth_.clear();

}

void Player::Initialize(Camera* camera)
{
	camera_ = camera;
	input_ = Input::GetInstance();

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Debug/Debug.obj", "Player");


	// 回転軸の初期化
	rotation_ = { 0.0f,0.0f,0.0f };

	/*===============================================================//
					 　　		スプライト
	//===============================================================*/
	uint32_t th[ch_] = { TextureManager::GetInstance()->Load("hpBar.png",defaulFilPath),
						 TextureManager::GetInstance()->Load("hp.png",defaulFilPath) };

	spriteHP_[0] = std::make_unique<Sprite>();
	spriteHP_[0].reset(Sprite::Create(th[0]));
	spriteHP_[0]->Initialize();
	spriteHP_[0]->SetAnchor({ 0.5f,0.5f });
	spriteHP_[0]->SetSize({ 200,100 });
	spriteHP_[0]->translate_ = { 933, 651 };


	spriteHP_[1] = std::make_unique<Sprite>();
	spriteHP_[1].reset(Sprite::Create(th[1]));
	spriteHP_[1]->Initialize();
	spriteHP_[1]->SetAnchor({ 1.0f,0.0f });
	spriteHP_[1]->SetSize({ 200,100 });
	spriteHP_[1]->translate_ = { 1033, 601 };


	/*===============================================================//
						 　　  コライダー設定
	//===============================================================*/

	collider_ = std::make_unique<Collider>();
	collider_->SetBoundingBox(Collider::BoundingBox::OBB_3D);
	collider_->SetShape(oModel_->GetMin(), oModel_->GetMax());
	collider_->SetAtrribute("Player");
	collider_->SetMask({ "Player" });
	collider_->SetGetWorldMatrixFunc([this]() { return oModel_->GetWorldTransform()->matWorld_; });
	collider_->SetOnCollisionFunc([this](const Collider* other) { OnCollision(other); });
	collider_->SetReferencePoint({ 0.0f, 0.0f, 0.0f });

	InitJsonBinder();

	gameTime_ = GameTime::GetInstance();
	gameTime_->CreateChannel("default");
}

void Player::Update()
{
	if (isAlive_)
	{

		collider_->RegsterCollider();

		Move();

		Rotate();

		Bulletdelete();

		Fire();

		UpdateHP();

		CameraShake();

		UpdateBullet();

		Knockback();

		CoolTimerBullet();

		ComboManager::GetInstance()->ResetCombo();
	}
	else if (isDeathEffectPlaying_)
	{
		UpdateDeathEffect();
	}

	oModel_->Update();

	worldPosition_ = GetWorldPosition();

#ifdef _DEBUG
	ImGui();
#endif // _DEBUG
}

void Player::Draw(const Vector4& color)
{
#ifdef _DEBUG
	collider_->Draw();

#endif // _DEBUG



	oModel_->Draw(camera_, color);



	for (NorthPoleBullet* bullet : bulletsNorth_) {
		bullet->Draw(*camera_, Vector4{ 255.0f,0.0f,0.0f,1.0f });
	}

	for (SouthPoleBullet* bullet : bulletsSouth_) {
		bullet->Draw(*camera_, Vector4{ 0.0f,0.0f,255.0f,1.0f });
	}


}

void Player::OnCollision(const Collider* other)
{
	// 無敵状態中なら衝突判定を無視する
	if (isKnockbackActive_) {
		return;
	}

	if (other->GetName() == "Enemy") {
		hp_-= takeDamage_;
		if (hp_ <= 0) {
			isAlive_ = false;
			BeginDeathEffect();
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

void Player::DrawSprite()
{
	Sprite::PreDraw();

	for (int i = 0; i < ch_; i++)
	{
		spriteHP_[i]->Draw();
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

void Player::UpdateHP()
{
	// HP割合を計算
	float hpRatio = std::clamp(hp_ / maxHp_, 0.0f, 1.0f);
	spriteHP_[1]->scale_ = { hpRatio, 1.0f };
	// UVスケールとUVトランスレートを設定
	spriteHP_[1]->uvScale_ = { hpRatio, 1.0f };
	spriteHP_[1]->uvTranslate_ = { 1.0f - hpRatio, 0.0f };
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
	if ((input_->IsPadTriggered(PadButton::iPad_RB) || input_->IsPadTriggered(PadButton::iPad_RT))
		&& northBulletCoolTimer_ <= 0.0f)
	{
		// クールタイムのリセット
		northBulletCoolTimer_ = bulletFireInterval_;

		// 前方ベクトルの取得
		Vector3 forward = GetForwardVector();

		// プレイヤーの前方に発射位置オフセット
		float spawnOffset = offset;
		Vector3 spawnPosition = GetWorldPosition() + forward * spawnOffset;

		// 速度・加速度の計算
		Vector3 velocity = forward * bulletVelocity_;
		Vector3 acceleration = forward * bulletAcceleration_;

		// 弾の生成および初期化
		NorthPoleBullet* newBullet = new NorthPoleBullet();
		newBullet->Initialize("Sphere/sphere.obj", "North", spawnPosition, velocity, acceleration);

		bulletsNorth_.push_back(newBullet);

		enableShake_ = true;
	}
}

void Player::SouthPoleBulletFire()
{
	if ((input_->IsPadTriggered(PadButton::iPad_LB) || input_->IsPadTriggered(PadButton::iPad_LT))
		&& southBulletCoolTimer_ <= 0.0f) {
		// クールタイムのリセット
		southBulletCoolTimer_ = bulletFireInterval_;

		// 前方ベクトルの取得
		Vector3 forward = GetForwardVector();

		// プレイヤーの前方に発射位置オフセット
		float spawnOffset = offset;
		Vector3 spawnPosition = GetWorldPosition() + forward * spawnOffset;

		// 速度・加速度の計算
		Vector3 velocity = forward * bulletVelocity_;
		Vector3 acceleration = forward * bulletAcceleration_;

		// 弾の生成および初期化
		SouthPoleBullet* newBullet = new SouthPoleBullet();
		newBullet->Initialize("Sphere/sphere.obj", "South", spawnPosition, velocity, acceleration);

		bulletsSouth_.push_back(newBullet);

		enableShake_ = true;
	}
}

void Player::CoolTimerBullet()
{
	// 発射のクールタイマーの更新
	const float deltaTime = 1.0f / 60.0f;
	if (northBulletCoolTimer_ > 0.0f) {
		northBulletCoolTimer_ -= deltaTime;
	}
	if (southBulletCoolTimer_ > 0.0f) {
		southBulletCoolTimer_ -= deltaTime;
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
	ImGui::DragFloat("Bullet Offset", &offset, 1.0f, 1.0f, 10.0f);
	ImGui::Separator();

	// 弾のクールダウンとインターバル
	ImGui::Text("BulletCoolTime Info");
	ImGui::DragFloat("BulletFire Interval", &bulletFireInterval_, 0.1f, 3.0f);
	ImGui::DragFloat("NorthBullet CoolTime", &northBulletCoolTimer_, 0.0f, 1.0f);
	ImGui::DragFloat("SouthBullet CoolTime", &southBulletCoolTimer_, 0.0f, 1.0f);

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

	ImGui::SeparatorText("Camera Shake");
	ImGui::DragFloat("time", &shakeTime_, 0.01f);
	ImGui::DragFloat2("rangeMin", &shakeRangeMin_.x, 0.01f);
	ImGui::DragFloat2("rangeMax", &shakeRangeMax_.x, 0.01f);


	ImGui::Separator();
	if (ImGui::Button("deathEffect"))
	{
		BeginDeathEffect();
	}

	ImGui::Separator();
	if (ImGui::Button("Save Settings"))
	{
		Save();
	}

	ImGui::End();
}
#endif


Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = oModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldPos.y = oModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldPos.z = oModel_->GetWorldTransform()->matWorld_.m[3][2];
	worldPosition_ = worldPos;

	return worldPos;
}

Vector3 Player::GetCenterPosition()
{
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, oModel_->GetWorldTransform()->matWorld_);

	return worldPos;
}

Vector3 Player::GetForwardVector() const
{
	// rotation_.y をもとに計算（例：XZ平面）
	float angle = rotation_.y;
	return Vector3{ sinf(angle), 0.0f, cosf(angle) };
}

Vector3* Player::GetWorldPositionRef()
{
	return &worldPosition_;
}

void Player::CameraShake()
{
	if (enableShake_) {
		// シェイクの更新
		camera_->Shake(shakeTime_, shakeRangeMin_, shakeRangeMax_);
		enableShake_ = false;
	}
}

void Player::BeginDeathEffect()
{
	isAlive_ = false;
	isDeathEffectPlaying_ = true;
	deathEffectParams_.duration = 0.0f;
	deathEffectParams_.beforePosition = oModel_->translate_;
	deathEffectParams_.shake = true;
	gameTime_->GetChannel("default").SetGameSpeed(0.25f);
}

void Player::UpdateDeathEffect()
{
	// TODO: 死亡演出の実装
	// 仮 プルプルさせてから拡大

	deathEffectParams_.duration += gameTime_->GetUnScaleDeltaTime_float();

	// プルプル
	if (deathEffectParams_.duration >= deathEffectParams_.shakeTime && deathEffectParams_.shake)
	{
		deathEffectParams_.shake = false;
		deathEffectParams_.duration = 0;
		deathEffectParams_.scale = true;
	}
	else if (deathEffectParams_.duration >= deathEffectParams_.scalingTIme && deathEffectParams_.scale)
	{
		deathEffectParams_.scale = false;
		deathEffectParams_.duration = 0;
		deathEffectParams_.wait = true;
	}
	else if (deathEffectParams_.duration >= deathEffectParams_.waitTime && deathEffectParams_.wait)
	{
		deathEffectParams_.wait = false;
		deathEffectParams_.duration = 0;
	}

	if (deathEffectParams_.shake)
	{
		// モデルをプルプルさせる
		Vector3 shakeDir = RandomGenerator::GetInstance()->GetUniformVec3((-1.0f, 0, -1.0f), (1.0f, 0, 1.0f));
		oModel_->translate_ = deathEffectParams_.beforePosition + shakeDir * deathEffectParams_.shakePower;
	}
	else if (deathEffectParams_.scale)
	{
		float t = deathEffectParams_.duration / deathEffectParams_.scalingTIme;
		// 縮小
		float easedT = Easing::EaseOutExpo(t);

		float scale = Lerp(1.0f, deathEffectParams_.targetScale, easedT);

		// tの閾値
		constexpr float threshold = 0.70f;

		if (t >= threshold)
		{
			scale = 0;
		}

		oModel_->scale_ = { scale, 1.0f, scale };
	}
	else if (deathEffectParams_.wait)
	{
	}
	else
	{
		// 死亡演出終了
		isDeathEffectPlaying_ = false;
		gameTime_->GetChannel("default").SetGameSpeed(1.0f);
	}

}

void Player::InitJsonBinder()
{


	jsonBinder_ = std::make_unique<JsonBinder>("PlayerData", "Resources/Data/Parameter/");

	// HP 関連
	jsonBinder_->RegisterVariable("MaxHP", &maxHp_);
	//jsonBinder_->RegisterVariable("HP", &hp_);
	// 移動関連
	jsonBinder_->RegisterVariable("CharacterSpeed", &kCharacterSpeed_);
	jsonBinder_->RegisterVariable("L_Stick DeadZone", &kDeadZoneL_);
	// 弾関連
	jsonBinder_->RegisterVariable("BulletVelocity", &bulletVelocity_);
	jsonBinder_->RegisterVariable("BulletAcceleration", &bulletAcceleration_);
	jsonBinder_->RegisterVariable("BulletOffset", &offset);
	jsonBinder_->RegisterVariable("BulletFireInterval", &bulletFireInterval_);
	// ノックバック関連
	jsonBinder_->RegisterVariable("KnockbackStrength", const_cast<float*>(&knockbackStrength_));
	jsonBinder_->RegisterVariable("KnockbackDamping", const_cast<float*>(&knockbackDamping_));
	jsonBinder_->RegisterVariable("KnockbackInvincibleDuration", const_cast<float*>(&knockbackInvincibleDuration_));

	jsonBinder_->RegisterVariable("ShakeTime", &shakeTime_);
	jsonBinder_->RegisterVariable("ShakeRangeMin", &shakeRangeMin_);
	jsonBinder_->RegisterVariable("ShakeRangeMax", &shakeRangeMax_);
}

void Player::Save()
{
	jsonBinder_->Save();
}