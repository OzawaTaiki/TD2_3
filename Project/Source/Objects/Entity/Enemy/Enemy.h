#pragma once

#include "../BaseEntity.h"
#include <Physics/Collision/Collider.h>


class Player;
class Enemy : public BaseEntity
{
public:

	// 弾のタイプを表す列挙型
	enum class BulletType {
		None,
		North,
		South
	};

	enum class MoveType {
		Direct,
		Target
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

	/// <summary>
	/// 属性の切り替え
	/// </summary>
	/// <param name="deltaTime"></param>
	void ChangeType(float deltaTime);


	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="deltaTime"></param>
	void Move(float& deltaTime);


public:
	// 現在の弾タイプを取得または設定
	BulletType GetCurrentType() const { return currentType_; }
	void SetCurrentType(BulletType type) { currentType_ = type; }


	// 現在の弾タイプを文字列で取得
	std::string GetCurrentTypeName() const {
		switch (currentType_) {
		case BulletType::None:  return "None";
		case BulletType::North: return "North";
		case BulletType::South: return "South";
		}
		return "Unknown";
	}

	// 移動のタイプをセット
	void SetMoveType(const std::string& moveType) {
		if (moveType == "Direct") {
			moveType_ = MoveType::Direct;
		}
		else if (moveType == "Target") {
			moveType_ = MoveType::Target;
		}
		else {
			moveType_ = MoveType::Direct; // デフォルト
		}
	};

private:

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(const Collider* other);

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGui();


public:

	Vector3& GetTranslate() { return oModel_->translate_; }
	Vector3 GetCenterPosition() const;

	void SetPlayer(Player* player) { player_ = player; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	void SetSpeed(const float& speed) { speed_ = speed; }
	void SetGoal(const Vector3& goal) { goal_ = goal; }
	void SetIsAlive(bool alive) { isAlive_ = alive; }
	bool& GetIsDraw() { return isDraw_; }
	void SetIsDraw(bool draw) { isDraw_ = draw; }
	void SetTranslate(Vector3& translate) { oModel_->translate_ = translate; }

	bool& GetIsAlive() { return isAlive_; }
	bool& GetIsAddScore() { return isAddScore_; }
	bool& GetMarkForRemoval() { return markForRemoval_; }
	
    Collider* getcoll() { return collider_.get(); }

private:
	
	/*===============================================================//
						 　　	ポインタなど
	//===============================================================*/

	std::unique_ptr<Collider> collider_ = nullptr;
	Player* player_ = nullptr;


	/*===============================================================//
					 　　		敵の情報
	//===============================================================*/

	//敵タイプ
	BulletType currentType_ = BulletType::None;
	MoveType moveType_ = MoveType::Direct;
	// 消滅するか
	bool markForRemoval_ = false;
	Vector3 velocity_;
	float speed_;
	Vector3 goal_;
	bool isDraw_ = true;
	bool isAlive_ = true;
	bool isAddScore_ = false;

	// 時間でNone状態に変更
	float typeChangeCount_ = 0.0f;
	const float typeChangeTime_ = 4.0f;
};

