#include "Player.h"

#include <algorithm>
void Player::Initialize()
{
	input_ = Input::GetInstance();

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Sphere/Sphere.obj", "Player");

	jsonBinder_ = std::make_unique<JsonBinder>("PlayerData", "Resources/Data");

}

void Player::Update()
{
	Move();

	oModel_->Update();



#ifdef _DEBUG
	ImGui();
#endif // _DEBUG
}

void Player::Draw(const Camera& camera, const Vector4& color)
{
	oModel_->Draw(&camera, color);
}

void Player::Move()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	

	if (input_->IsControllerConnected()) {
		// パッドの左スティック（アナログ入力）による移動
		XINPUT_STATE xInputState;
		ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

		// ゲームパッドの状態を取得
		if (XInputGetState(0, &xInputState) == ERROR_SUCCESS) {
			// 左スティックのX軸とY軸の値を取得（-32768 ～ 32767）
			float thumbLX = static_cast<float>(xInputState.Gamepad.sThumbLX);
			float thumbLY = static_cast<float>(xInputState.Gamepad.sThumbLY);


			// 左スティックのX軸を移動に反映（X軸方向）
			if (fabs(thumbLX) > kDeadZone_) {
				move.x += (thumbLX / 32767.0f) * kCharacterSpeed_;
			}

			// 左スティックのY軸を移動に反映（Z軸方向）
			if (fabs(thumbLY) > kDeadZone_) {
				move.z += (thumbLY / 32767.0f) * kCharacterSpeed_;
			}
		}

		// ワールド座標に移動を反映
		oModel_->translate_ += move;

		// ----- 移動限界座標 ----- //
		//const float kMoveLimitX = 34.0f;
		//const float kMoveLimitZ = 18.0f;

		//// 範囲を超えない処理
		//oModel_->translate_.x = std::clamp(oModel_->translate_.x, -kMoveLimitX, kMoveLimitX);
		//oModel_->translate_.z = std::clamp(oModel_->translate_.z, -kMoveLimitZ, kMoveLimitZ);
	}
}

void Player::Fire()
{


}

void Player::Save()
{

}

void Player::ImGui()
{
	ImGui::Begin("Player");

	ImGui::DragFloat("DeadZone", &kDeadZone_);
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