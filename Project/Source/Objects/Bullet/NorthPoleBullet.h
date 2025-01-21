#pragma once
#include "BaseBullet.h"

class NorthPoleBullet : public BaseBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& directoryPath, const std::string& name, const Vector3& translate, const Vector3& velocity, const Vector3& acceleration) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera, const Vector4& color) override;

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(const Collider* other) override;
};

