#pragma once
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>

#include <Systems/Input/Input.h>
#include <Systems/JsonBinder/JsonBinder.h>

#include "../../Bullet/SouthPoleBullet.h"
#include "../../Bullet/NorthPoleBullet.h"
#include "../BaseEntity.h"
#include <Physics/Collision/Collider.h>

// C++
#include <memory>
#include <list>


class Player : public BaseEntity
{
public:
	~Player();

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
	/// 衝突
	/// </summary>
	void OnCollision(const Collider* other);


private:

	/// <summary>
	/// 移動関数
	/// </summary>
	void Move();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 全体射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// ノックバックの計算処理
	/// </summary>
	void Knockback();


	/// <summary>
	/// N極のタイプの弾
	/// </summary>
	void NorthPoleBulletFire();

	/// <summary>
	/// S極のタイプの弾
	/// </summary>
	void SouthPoleBulletFire();

	/// <summary>
	/// 弾発射のクールタイム
	/// </summary>
	void CoolTimerBullet();

	/// <summary>
	/// 弾の更新
	/// </summary>
	void UpdateBullet();

	/// <summary>
	/// 弾の削除
	/// </summary>
	void Bulletdelete();

    /// <summary>
    /// カメラシェイク
    /// </summary>
    void CameraShake();

    /// <summary>
    /// JsonBinderの初期化
    /// </summary>
	void InitJsonBinder();
	void Save();


#ifdef _DEBUG
	void ImGui();
#endif // _DEBUG

public:
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();


	/// <summary>
	/// ワールドトランスフォーム
	/// </summary>
	const WorldTransform& GetWorldTransform() {return *oModel_->GetWorldTransform();}

	/// <summary>
	/// 中心点の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition();

	/// <summary>
	/// プレイヤーの回転角度から前方ベクトルを計算して返す関数（XZ平面）
	/// </summary>
	/// <returns>前方への単位ベクトル</returns>
	Vector3 GetForwardVector() const;

	/// <summary>
	/// 生存フラグを取得
	/// </summary>
	/// <returns></returns>
	bool IsAlive()const { return isAlive_; }

    /// <summary>
    /// プレイヤーの座標を設定
    /// </summary>
    /// <param name="_translate"></param>
    void SetTranslate(const Vector3& _translate) { oModel_->translate_ = _translate; }


    Vector3* GetWorldPositionRef();

private:
	/*===============================================================//
							 　　ポインタなど
	//===============================================================*/
	Input* input_ = nullptr;
	std::unique_ptr<Collider> collider_ = nullptr;
	std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

	std::list<NorthPoleBullet*> bulletsNorth_;
	std::list<SouthPoleBullet*> bulletsSouth_;


	/*===============================================================//
								コントローラー
	//===============================================================*/

	// デッドゾーンを設定
	Vector3 rotation_;
	float kDeadZoneL_ = 7000.0f;
	float kDeadZoneR_ = 10000.0f;
	float kCharacterSpeed_ = 0.25f;
	float kRotationSpeed_ = 0.05f;

	// 移動量を保存
	Vector3 prePosition_= { 0.0f,0.0f,0.0f };




	/*===============================================================//
								 弾関連
	//===============================================================*/
	float bulletVelocity_ =  0.03f;
	float bulletAcceleration_ = 0.015f;
	float offset = 2.0f;

	//--------------- 弾のクールタイム ---------------//

	// 弾の発射間隔
	float bulletFireInterval_ = 1.0f;

	// 発射クールダウンタイマー
	float northBulletCoolTimer_ = 0.0f;
	float southBulletCoolTimer_ = 0.0f;



    /*===============================================================//
								HPなど
    //===============================================================*/
    float maxHp_ = 100.0f;
    float hp_ = 100.0f;
    bool isAlive_ = true;

	/*===============================================================//
							ノックバックの処理
	//===============================================================*/

	// ノックバックの初期速度
	Vector3 knockbackVelocity_ = { 0.0f, 0.0f, 0.0f };
	// イージングの減衰係数		0.0～1.0、1.0なら即刻停止、0.0なら全く減衰しない
	const float knockbackDamping_ = 0.9f;
	// ノックバックの強さ
	const float knockbackStrength_ = 1.0f;

	// ノックバック中かどうかを示すフラグ
	bool isKnockbackActive_ = false;
	// 無敵状態の残り時間
	float knockbackInvincibleTime_ = 0.0f;
	// 無敵状態の継続時間（調整可能）
	const float knockbackInvincibleDuration_ = 0.5f;

    //===============================================================//
	//					カメラシェイクのパラメータ						 //
    //===============================================================//
    Vector2 shakeRangeMin_ = { -0.1f, -0.1f };
    Vector2 shakeRangeMax_ = { 0.1f, 0.1f };
    float shakeTime_ = 0.1f;
    bool enableShake_ = false;

	// ライト参照用 ワールド座標
	Vector3 worldPosition_ = {};
};

