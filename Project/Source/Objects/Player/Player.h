#pragma once
#include <Rendering/Model/ObjectModel.h>
#include <Rendering/Model/AnimationModel.h>
#include <Framework/Camera/Camera.h>

#include <Systems/Input/Input.h>

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

#ifdef _DEBUG
	void ImGui();
#endif // _DEBUG

private:
	Input* input_ = nullptr;
	std::unique_ptr<ObjectModel> oModel_ = nullptr;

};

