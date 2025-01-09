#include "NorthPoleBullet.h"

void NorthPoleBullet::Initialize(const std::string& directoryPath, const std::string& name, const Vector3& translate, const Vector3& velocity)
{
	BaseBullet::Initialize(directoryPath, name,translate, velocity);


}

void NorthPoleBullet::Update()
{
	BaseBullet::Update();
}

void NorthPoleBullet::Draw(const Camera& camera, const Vector4& color)
{
	BaseBullet::Draw(camera, color);
}
