#include "TitleEnemy.h"


void TitleEnemy::Initialize(Camera* _camera, const Vector3& _start, const Vector3& _end)
{
    Enemy::Initialize(_camera);

    translate_ = &Enemy::GetTranslate();

    start_ = _start;
    end_ = _end;
}

void TitleEnemy::Update()
{
    translate_->x += moveSpeed_;

    // ru-pu
    if (translate_->x < end_.x)
    {
        translate_->x = start_.x;
        Enemy::GetIsAlive() = false;
        return;
    }

    else if (translate_->x > start_.x)
    {
        translate_->x = end_.x;
        Enemy::GetIsAlive() = false;
        return;
    }

    Enemy::Update();

}

void TitleEnemy::Draw(const Vector4& _color)
{
    Enemy::Draw(_color);
}
