#include "Enemy.h"

void Enemy::Initialize(Camera* camera)
{
	camera_ = camera;

	oModel_ = std::make_unique<ObjectModel>();
	oModel_->Initialize("human/walk.gltf", "Enemy");
}

void Enemy::Update()
{
	oModel_->Update();
}

void Enemy::Draw(const Vector4& color)
{
	oModel_->Draw(camera_, color);
}
