
#include <Rendering/Sprite/Sprite.h>
#include <memory>
#include <Framework/eScene/BaseScene.h>
#include <Systems/UVTransform/SpriteSheetAnimetion.h>

class ResultScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    ~ResultScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    void DrawScore();
#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG


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

    uint32_t scoreTh;
    std::vector<std::unique_ptr<Sprite>> SpriteCopy_;
    std::array<std::unique_ptr<Sprite>, 10> scoreSprites_;
	std::unique_ptr<Sprite> comboSprite_;
	std::unique_ptr<Sprite> countSprite_;


};