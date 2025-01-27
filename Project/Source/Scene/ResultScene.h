
#include <Rendering/Sprite/Sprite.h>
#include <memory>
#include <Framework/eScene/BaseScene.h>
#include <Rendering/LineDrawer/LineDrawer.h>
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
    /*===============================================================//
                     　　         ポインタなど
    //===============================================================*/
    static const int ms_ = 3; 
    std::unique_ptr<Sprite> resultSprite_[ms_];

    /*===============================================================//
                 　　          外部のスプライト
    //===============================================================*/
	int score_ = 0;
	int combo_ = 0;
	int count_ = 0;

    //uint32_t scoreTh;
    std::array<std::unique_ptr<Sprite>, 10> scoreSprites_;
	std::array<std::unique_ptr<Sprite>, 10> comboSprites_;
	std::array<std::unique_ptr<Sprite>, 10> countSprites_;


};