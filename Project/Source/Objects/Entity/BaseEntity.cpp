#include "BaseEntity.h"

Vector3 BaseEntity::GetWorldPosition() const
{
    Vector3 worldPos;
    if (oModel_) {
        worldPos.x = oModel_->GetWorldTransform()->matWorld_.m[3][0];
        worldPos.y = oModel_->GetWorldTransform()->matWorld_.m[3][1];
        worldPos.z = oModel_->GetWorldTransform()->matWorld_.m[3][2];
    }
    return worldPos;
}
