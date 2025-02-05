#pragma once

#include <UI/UISprite.h>

class TItleUI
{
public:
    TItleUI() = default;
    ~TItleUI();

    void Initialize();
    void Update();
    void Draw();

private:

    UISprite moveGuideSprite_ = {};
    UISprite shotGuideSprite_= {};
};
