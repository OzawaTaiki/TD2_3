#pragma once

#include "../BaseEntity.h"
#include <Physics/Collision/Collider.h>
#include <Systems/Audio/AudioSystem.h>

#include "OffScreenEnemyMark.h"

class GameScene;
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

	void DrawInfuenceCircle();

    void DrawSprite();

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

	void SetHitSound(uint32_t _handle, float _volume, float _startOffset) {
		hitHandle_ = _handle;
		hitVolume_ = _volume;
		hitStartOffset_ = _startOffset;
	}

	void SetDeathSound(uint32_t _handle, float _volume, float _startOffset) {
		deathHandle_ = _handle;
		deathVolume_ = _volume;
		deathStartOffset_ = _startOffset;
	}

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	
	/*===============================================================//
						 　　	ポインタなど
	//===============================================================*/

	std::unique_ptr<Collider> collider_ = nullptr;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

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

	int score_ = 100;

	// 時間でNone状態に変更
	float typeChangeCount_ = 0.0f;
	const float typeChangeTime_ = 1000.0f;

    std::unique_ptr<OffScreenEnemyMark> offScreenMark_ = nullptr;

	// サウンド
    uint32_t hitHandle_;
    VoiceHandle hitVoice_;
    float hitVolume_ = 1.0f;
    float hitStartOffset_ = 0.0f;

    uint32_t deathHandle_;
    VoiceHandle deathVoice_;
    float deathVolume_ = 1.0f;
    float deathStartOffset_ = 0.0f;

    std::unique_ptr<ObjectModel> circle_ = nullptr;
    uint32_t infulenceRedTexture_ = 0;
    uint32_t infulenceBlueTexture_ = 0;
};

