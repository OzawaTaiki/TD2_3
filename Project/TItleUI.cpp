#include "TItleUI.h"

TItleUI::~TItleUI()
{
}

void TItleUI::Initialize()
{
    moveGuideSprite_.Initialize("MoveGuide");

    shotGuideSprite_.Initialize("ShotGuide");
}

void TItleUI::Update()
{
    moveGuideSprite_.Update();
    shotGuideSprite_.Update();
}

void TItleUI::Draw()
{
    moveGuideSprite_.Draw();
    shotGuideSprite_.Draw();
}
