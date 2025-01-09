#pragma once
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>
#include <Framework/Camera/Camera.h>
#include <Systems/Input/Input.h>

// C++
#include <memory>
class PlayerBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3& translate,Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera, const Vector4& color);

private:

#ifdef _DEBUG
	void ImGui();
#endif // _DEBUG


public:

	/// <summary>
	/// デスタイマー関数
	/// </summary>
	bool IsAlive() const { return isAlive_; }

	
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

private:
	// モデルデータ
	std::unique_ptr<ObjectModel> oModel_ = nullptr;

	// 移動速度
	Vector3 velocity_ = Vector3(0.0f, 0.0f, 0.0f);

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;

	// 生存フラグ
	bool isAlive_ = true;


private:
	// 静的メンバ変数
	static const int32_t kLifeTime = 60 * 5; // 寿命<frm>
	static inline const float rad = 1.0f;    // 半径

};

