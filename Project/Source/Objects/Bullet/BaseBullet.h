#pragma once
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>
#include <Framework/Camera/Camera.h>
#include <Systems/Input/Input.h>
#include <Physics/Collision/Collider.h>

// C++
#include <memory>
class BaseBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const std::string& directoryPath, const std::string& name,
		const Vector3& translate, const Vector3& velocity,const Vector3& acceleration);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Camera& camera, const Vector4& color);

	/// <summary>
	/// 衝突
	/// </summary>
	virtual void OnCollision(const Collider* other) = 0;
public:

	/// <summary>
	/// デスタイマー関数
	/// </summary>
	virtual bool IsAlive() const { return isAlive_; }


	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	virtual Vector3 GetWorldPosition();

protected:
	/*===============================================================//
					 　　		ポインタなど
	//===============================================================*/
	std::unique_ptr<ObjectModel> oModel_ = nullptr;
	std::unique_ptr<Collider> collider_ = nullptr;


	/*===============================================================//
					 　　		その他設定
	//===============================================================*/

	// 移動速度
	Vector3 velocity_ = Vector3(0.0f, 0.0f, 0.0f);
	// 加速度
	Vector3 acceleration_ = Vector3(0.0f, 0.0f, 0.0f);
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// 生存フラグ
	bool isAlive_ = true;
	// 一定割合で補間する。
	float easeRate_ = 1.2f; 

private:
	// 静的メンバ変数
	static const int32_t kLifeTime = 60 * 5; // 寿命<frm>
	static inline const float rad = 1.0f;    // 半径

};

