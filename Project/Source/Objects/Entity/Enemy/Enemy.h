#pragma once

#include "../BaseEntity.h"
#include <Physics/Collision/Collider.h>

class Enemy : public BaseEntity
{
public:

	// 弾のタイプを表す列挙型
	enum class BulletType {
		None,
		North,
		South
	};

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


	// 現在の弾タイプを取得または設定
	BulletType GetCurrentType() const { return currentType_; }
	void SetCurrentType(BulletType type) { currentType_ = type; }


	// デバッグ用: 現在の弾タイプを文字列で取得
	std::string GetCurrentTypeName() const {
		switch (currentType_) {
		case BulletType::None:  return "None";
		case BulletType::North: return "North";
		case BulletType::South: return "South";
		}
		return "Unknown";
	}


private:

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(const Collider* other);





public:

	Vector3& GetTranslate() { return oModel_->translate_; }
	Vector3 GetCenterPosition() const;
	void SetTranslate(Vector3& translate) { oModel_->translate_ = translate; }
	bool& GetIsAlive() { return isAlive_; }
    bool& GetMarkForRemoval() { return markForRemoval_; }
    Collider* getcoll() { return collider_.get(); }

private:
	
	/*===============================================================//
						 　　	ポインタなど
	//===============================================================*/

	std::unique_ptr<Collider> collider_ = nullptr;
	bool isAlive_ = true;

    // マークされているか
	// 消滅するか
    bool markForRemoval_ = false;

	// 現在の弾タイプ
	BulletType currentType_ = BulletType::None;
};

