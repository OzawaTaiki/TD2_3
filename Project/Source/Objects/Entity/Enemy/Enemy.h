#pragma once

#include "../BaseEntity.h"
class Enemy : public BaseEntity
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Vector4& color) override;

public:


	bool& GetIsAlive() { return isAlive_; }

private:
	
	bool isAlive_ = true;

};

