
#include <Rendering/Sprite/Sprite.h>
#include <memory>
#include <Framework/eScene/BaseScene.h>
#include <Rendering/LineDrawer/LineDrawer.h>
#include <Systems/Audio/AudioSystem.h>
#include <UI/UISprite.h>
#include "../System_TD/ScoreManager/ScoreManager.h"
#include "../System_TD/ComboManager/ComboManager.h"
#include "../System_TD/CountManager/CountManager.h"

class ResultScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    ~ResultScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:

    /// <summary>
    /// スコアの描画
    /// </summary>
    void DrawScore();

	/// <summary>
	/// コンボの描画
	/// </summary>
	void DrawCombo();

    /// <summary>
    /// 倒した敵の数の描画
    /// </summary>
    void DrawCountEnemy();

private:

    void UpdateNumbers();

    /*===============================================================//
                     　　         ポインタなど
    //===============================================================*/
    static const int ms_ = 4;
    Sprite* resultSprite_[ms_];

    AudioSystem* audio_;

    /*===============================================================//
                 　　          外部のスプライト
    //===============================================================*/
	int score_ = 0;
	int combo_ = 0;
	int count_ = 0;

    float timePerDigit_ = 0.5f;
    float duration_ = 0.0f;
    int currentDigitIndex = 0;

    int targetScore_ = 0;
    int targetCount_ = 0;
    int targetCombo_ = 0;

    //uint32_t scoreTh;
    std::array<Sprite*, 10> scoreSprites_;
	std::array<Sprite*, 10> comboSprites_;
	std::array<Sprite*, 10> countSprites_;


    /*===============================================================//
             　　                 UI
    //===============================================================*/

    UISprite uiTitle_;
    UISprite uiA_;
    uint32_t UI_TX[2];
    bool isRetry_ = true;

    bool drawSelect_ = false;


    // サウンド
    uint32_t bgmHandle_ = 0;
    uint32_t drumrollHandle_ = 0;
    uint32_t drumrollEndHandle_ = 0;
    uint32_t buttonHandle_ = 0;

    VoiceHandle bgmVoice_;
    VoiceHandle drumrollVoice_;
    VoiceHandle drumrollEndVoice_;
    VoiceHandle buttonVoice_;

    // オフセット
    float buttonOffset = 0.2f;
};