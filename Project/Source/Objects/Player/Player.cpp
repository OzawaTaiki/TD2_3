#include "Player.h"

void Player::Initialize()
{
	input_ = Input::GetInstance();

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("Sphere/Sphere.obj","d");

}

void Player::Update()
{
	oModel_->Update();
}

void Player::Draw(const Camera& camera, const Vector4& color)
{
	oModel_->Draw(&camera, color);
}

void Player::ImGui()
{
}
