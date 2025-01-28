#include "TitleEnemy.h"


void TitleEnemy::Initialize(Camera* camera)
{
    Enemy::Initialize(camera);

    translate_ = &Enemy::GetTranslate();

    start_ = { 10.0f,0.0f,0.0f };
    end_ = { -10.0f,0.0f,0.0f };

}

void TitleEnemy::Update()
{
    translate_->x += moveSpeed_;

    // ru-pu
    if (translate_->x < end_.x)
        translate_->x = start_.x;

    else if (translate_->x > start_.x)
        translate_->x = end_.x;

    Enemy::Update();

}

void TitleEnemy::Draw(const Vector4& _color)
{
    Enemy::Draw(_color);
}
