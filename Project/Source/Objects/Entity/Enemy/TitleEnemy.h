#pragma once

#include "Enemy.h"

class TitleEnemy : public Enemy
{
public:
    TitleEnemy() = default;
    ~TitleEnemy() override = default;

    void Initialize(Camera* _camera) override;

    void Update() override;

    void Draw(const Vector4& _color);

    void SetTranslate(const Vector3& _translate) { *translate_ = _translate; }

    void SetMoveSpeed(float _speed) { moveSpeed_ = _speed; }

    void SetRangeOfMovement(const Vector3& _start, const Vector3& _end) {
        start_ = _start;
        end_ = _end;
    }

private:

    float moveSpeed_ = -0.1f;

    Vector3 start_;
    Vector3 end_;

    Vector3* translate_;


};
