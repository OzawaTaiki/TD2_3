#pragma once

#include "../BaseEntity.h"
#include <Physics/Collision/Collider.h>

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


private:

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(const Collider* other);


public:


	bool& GetIsAlive() { return isAlive_; }

private:
	
	/*===============================================================//
						 　　	ポインタなど
	//===============================================================*/

	std::unique_ptr<Collider> collider_ = nullptr;
	bool isAlive_ = true;

};

