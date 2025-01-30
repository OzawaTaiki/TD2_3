#pragma once
#include <Framework/eScene/BaseScene.h>

#include <Framework/Camera/Camera.h>
#include <Framework/Camera/DebugCamera.h>
#include <Rendering/Light/LightGroup.h>
#include <Systems/Audio/AudioSystem.h>
#include <memory>

#include "../Objects/Entity/Player/Player.h"
#include "../Objects/Entity/Enemy/EnemyManager.h"
#include "../System_TD/Camera/FollowCamera.h"
#include "Systems/Time/GameTime.h"
#include <Physics/Collision/CollisionManager.h>
#include "LoadScene.h"
#include <Source/Objects/Area/Area.h>
#include <thread>


struct BonusScore {
    Sprite* sprite; // スプライト
    Vector2 position; // 現在位置
    float lifetime; // 表示時間
    bool active; // 表示中かどうか
};


class GameScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

     ~GameScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    void Load();
    void DrawScore();
    void DrawCombo();

    /*===============================================================//
                     　　      ポインタなど
    //===============================================================*/
    std::atomic<bool> Loading_ = true;
    std::unique_ptr<std::thread> loadThread_;
    Camera SceneCamera_ = {};
    DebugCamera debugCamera_ = {};
    FollowCamera followCamera_ = {};
    CollisionManager* collisionManager_ = nullptr;
    bool enableDebugCamera_ = false;
    LightGroup lightGroup_ = {};
    LoadScene* loadScene_ = nullptr;

    Area* area_ = nullptr;
    std::unique_ptr<Player> player_ = nullptr;
    std::unique_ptr<EnemyManager> enemyManager_ = nullptr;


    /*===============================================================//
                 　　             時間
    //===============================================================*/
	GameTime* gameTime_ = nullptr;
    float time_ = 0.0f;

    /*===============================================================//
                     　　         スプライト
    //===============================================================*/
    static const int bg_ = 2;
    Sprite* backGrounds_[bg_];

    int scoreCount_ = 0;
    int comboCount_ = 0;

    std::array<Sprite*, 11> comboSprites_;
    std::array<Sprite*, 10> scoreSprites_;

    std::vector<BonusScore> bonusScores_; // 「+100」の管理リスト
    const float bonusLifetime_ = 1.0f;    // 「+100」の表示時間

    // サウンド
    AudioSystem* audio_ = nullptr;

    uint32_t bgmHandle_ = 0;
    VoiceHandle bgmVoice_ = {};

    uint32_t countHandle_ = 0;
    VoiceHandle countVoice_ = {};

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG
};