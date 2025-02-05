#pragma once

#include <Rendering/Sprite/Sprite.h>

class Camera;
class OffScreenEnemyMark
{
public:

    OffScreenEnemyMark()=default;
    ~OffScreenEnemyMark() = default;

    void Initialize();

    void Update(const Camera& _camera, const Vector3& _pos);

    void Draw();

private:

    Sprite* sprite_;

    bool isDraw_ = false;

    static const Vector2 leftTop_;
    static const Vector2 rightTop_;
    static const Vector2 leftBottom_;
    static const Vector2 rightBottom_;
    static const Vector2 center_;

};
