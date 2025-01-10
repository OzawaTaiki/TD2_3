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
	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f, 0.0f, 0.0f };
		Matrix4x4 CameraMatrix = MakeRotateMatrix(rotate_);
		offset = TransformNormal(offset, CameraMatrix);
		// 座標をコピーしてオフセット分ずらす
		translate_ = target_->transform_ + offset;

		// カメラのビュー行列を更新
		matView_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));
	}
}

void FollowCamera::GetViewProjection() const
{
}
