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
	/// N極のタイプの弾
	/// </summary>
	void NorthPoleBulletFire();

	/// <summary>
	/// S極のタイプの弾
	/// </summary>
	void SouthPoleBulletFire();

	/// <summary>
	/// 弾の更新
	/// </summary>
	void UpdateBullet();

	/// <summary>
	/// 弾の削除
	/// </summary>
	void Bulletdelete();


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

	Vector3 GetCenterPosition();

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
	float kDeadZoneL_ = 7000.0f;
	float kDeadZoneR_ = 10000.0f;
	float kCharacterSpeed_ = 0.25f;
	Vector3 rotation_; 
	float kRotationSpeed_ = 0.05f; 





	/*===============================================================//
								 弾関連
	//===============================================================*/
	float bulletVelocity_ =  1.0f;


};

