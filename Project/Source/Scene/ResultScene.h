
#include <Rendering/Sprite/Sprite.h>
#include <memory>
#include <Framework/eScene/BaseScene.h>

class ResultScene : public BaseScene
{
public:

    static std::unique_ptr<BaseScene> Create();

    ~ResultScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

private:

#ifdef _DEBUG
    void ImGui();
#endif // _DEBUG


private:
    /*===============================================================//
                     　　 ポインタなど
    //===============================================================*/
    static const int ms_ = 3; 
    std::unique_ptr<Sprite> resultSprite_[ms_];

};