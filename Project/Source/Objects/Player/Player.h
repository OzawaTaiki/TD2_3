#pragma once
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>
#include <Framework/Camera/Camera.h>

#include <Systems/Input/Input.h>
#include <Systems/JsonBinder/JsonBinder.h>

// C++
#include <memory>

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
	/// 射撃関数
	/// </summary>
	void Fire();


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
	Input* input_ = nullptr;
	std::unique_ptr<ObjectModel> oModel_ = nullptr;
	std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;

	// デッドゾーンを設定（小さい入力値を無視）
	float kDeadZone_ = 7000.0f;
	float kCharacterSpeed_ = 0.25f;

};

