#include "FollowCamera.h"
#include "Physics/Math/MatrixFunction.h"
#include <Physics/Math/VectorFunction.h>

void FollowCamera::Initialize()
{
}

void FollowCamera::Update()
{
	FollowProcess();
}

void FollowCamera::FollowProcess()
{
    if (target_) {
        // 真上に配置するオフセット
        Vector3 offset = { 0.0f, 50.0f, 0.0f }; // Y軸方向に100.0f分離れる

        // 真下を向くようにカメラを回転
        rotate_ = { DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f }; // X軸回転 -90度

        // ターゲット位置にオフセットを加算
        translate_ = target_->transform_ + offset;

        // ビュー行列を更新
        matView_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));
    }
	if (ImGui::CollapsingHeader("FollowCamera Debug")) {
		ImGui::Text("Translate");
		ImGui::DragFloat3("Translate", &translate_.x);

		ImGui::Text("Rotation");
		ImGui::DragFloat3("Rotation", &rotate_.x);

		ImGui::Text("Scale");
		ImGui::DragFloat3("Scale", &scale_.x);
	}
}

void FollowCamera::GetViewProjection() const
{
}
