#pragma once

#include "../../Objects/Entity/Enemy/Enemy.h"

class TypeManager
{
public:
    enum class BulletType {
        None,
        North,
        South
    };

    // 敵のタイプを変更する関数
    static void ApplyTypeChange(Enemy* enemy, BulletType bulletType) {
        if (enemy) {
            enemy->SetCurrentType(static_cast<Enemy::BulletType>(bulletType));
        }
    }

    // 弾が持つタイプを名前から判定する関数
    static BulletType GetBulletType(const std::string& bulletName) {
        if (bulletName == "NorthBullet") {
            return BulletType::North;
        }
        else if (bulletName == "SouthBullet") {
            return BulletType::South;
        }
        return BulletType::None;
    }
};
