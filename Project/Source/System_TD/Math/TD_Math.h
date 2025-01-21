#pragma once
#include <Physics/Math/Vector3.h>
#include <Physics/Math/Matrix4x4.h>
#include <Physics/Math/MatrixFunction.h>
#include <Physics/Math/Vector4.h>



Vector4 Multiply(const Vector4& vec, const Matrix4x4& mat);
/// <summary>
/// OBB内で使用する「ワールド座標 → ローカル座標」変換関数の例
/// </summary>
Vector3 WorldToLocal(const Vector3& worldPos, const Matrix4x4& worldMatrix);
/// <summary>
/// OBB内で使用する「ローカル座標 → ワールド座標」変換関数の例
/// </summary>
Vector3 LocalToWorld(const Vector3& localPos, const Matrix4x4& worldMatrix);

