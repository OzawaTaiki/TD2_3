#include "TD_Math.h"


Vector4 Multiply(const Vector4& vec, const Matrix4x4& mat) {
    float x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];
    float y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];
    float z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];
    float w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3];

    return Vector4(x, y, z, w);
}

/// <summary>
/// OBB内で使用する「ワールド座標 → ローカル座標」変換関数の例
/// </summary>

Vector3 WorldToLocal(const Vector3& worldPos, const Matrix4x4& worldMatrix)
{
    // ワールド行列の逆行列を求める
    Matrix4x4 invMatrix = Inverse(worldMatrix);

    // 4次元ベクトル(位置座標なのでw=1)
    Vector4 pos4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

    // 逆行列を掛ける（行列のかけ方はライブラリによって異なるので注意）
    Vector4 localPos4 = Multiply(pos4 , invMatrix);
    // ↑行・列優先の実装で順序が異なる場合があります。
    // 例: 逆行列 * pos4 になるライブラリもあるのでご利用の環境に合わせてください

    // w分を考慮し、最終的なxyzを返す
    // (基本的には localPos4.w が 1.0 に近い値になっているはず)
    return Vector3(localPos4.x, localPos4.y, localPos4.z);
}

/// <summary>
/// OBB内で使用する「ローカル座標 → ワールド座標」変換関数の例
/// </summary>

Vector3 LocalToWorld(const Vector3& localPos, const Matrix4x4& worldMatrix)
{
    // 4次元ベクトル(位置座標なのでw=1)
    Vector4 pos4(localPos.x, localPos.y, localPos.z, 1.0f);

    // ワールド行列を掛ける（行列のかけ方はライブラリによって異なる）
    Vector4 worldPos4 = Multiply(pos4 , worldMatrix);
    // もしくは worldMatrix * pos4 になる可能性もあります

    // w分を考慮し、最終的なxyzを返す
    return Vector3(worldPos4.x, worldPos4.y, worldPos4.z);
}
