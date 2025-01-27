#pragma once

#include <ISceneTransition.h>
#include <Rendering/Sprite/Sprite.h>
#include <Systems/Time/GameTime.h>

class SceneTransition : public ISceneTransition
{
public:
    SceneTransition() = default;
    ~SceneTransition() override = default;

    void Initialize() override;

    // 開始前の初期化
    void Start() override;
    // 更新
    void Update() override;
    // 描画
    void Draw() override;
    // 終了処理
    void End() override;
    // シーン遷移終了判定
    bool IsEnd() override { return isEnd_; }
    // シーン遷移可能か
    bool CanSwitch() override { return canSwitch_; }

private:

    // シーン遷移終了フラグ
    bool isEnd_;
    // シーン遷移可能フラグ
    bool canSwitch_;


    float radius_ = 0.0f;
    float maxRadius_ = 1500.0f;

    float duration_ = 0.0f;
    // かかる時間
    float time_ = 1.0f;


    Sprite* sprite_;

    GameTime* gameTime_;
    GameTimeChannel* channel_;

};
