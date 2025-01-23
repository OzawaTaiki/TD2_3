#include "FollowCamera.h"
#include "Physics/Math/MatrixFunction.h"
#include <Physics/Math/VectorFunction.h>

void FollowCamera::Initialize()
{
}

void FollowCamera::Update(bool _lock)
{
	FollowProcess(_lock);

#ifdef _DEBUG
	ImGui();
#endif // _DEBUG

}

void FollowCamera::FollowProcess(bool _lock)
{
	if (target_ && !_lock) {

		// 真下を向くようにカメラを回転
		rotate_ = { DirectX::XMConvertToRadians(90), 0.0f, 0.0f }; // X軸回転 -90度

		// ターゲット位置にオフセットを加算
		translate_ = target_->transform_ + offset_;

		// ビュー行列を更新
		matView_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));
	}

}

void FollowCamera::GetViewProjection() const
{
}

void FollowCamera::ImGui()
{
	ImGui::Begin("FollowCamera Info");
	ImGui::DragFloat3("OffSet", offset_);

	ImGui::DragFloat3("Translate", &translate_.x);

	ImGui::DragFloat3("Rotation", &rotate_.x);

	ImGui::DragFloat3("Scale", &scale_.x);

	ImGui::End();
}
