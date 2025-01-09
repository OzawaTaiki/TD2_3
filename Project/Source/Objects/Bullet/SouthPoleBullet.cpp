#include "SouthPoleBullet.h"

void SouthPoleBullet::Initialize(const std::string& directoryPath, const std::string& name, const Vector3& translate, const Vector3& velocity)
{
	BaseBullet::Initialize(directoryPath, name, translate, velocity);


}

void SouthPoleBullet::Update()
{
	BaseBullet::Update();
}

void SouthPoleBullet::Draw(const Camera& camera, const Vector4& color)
{
	BaseBullet::Draw(camera, color);
}
