#pragma once
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>
#include <Framework/Camera/Camera.h>

#include <Systems/Input/Input.h>
#include <Systems/JsonBinder/JsonBinder.h>


// C++
#include <memory>
#include "../Bullet/PlayerBullet.h"
#include <list>

class Player
{
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera,const Vector4& color);


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
	/// 射撃関数
	/// </summary>
	void Fire();

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

private:
	/*===============================================================//
							 　　ポインタなど
	//===============================================================*/
	Input* input_ = nullptr;

	std::unique_ptr<ObjectModel> oModel_ = nullptr;
	std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

	std::list<PlayerBullet*> bullets_;





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
	float bulletVelocity_ =  3.0f;


};

