#pragma once



#include <cstdint>
#include <memory>
#include <Systems/JsonBinder/JsonBinder.h>
#include <Rendering/Sprite/Sprite.h>


class ComboManager
{
public:
    static ComboManager* GetInstance();
    ComboManager(const ComboManager&) = delete;
    ComboManager& operator=(const ComboManager&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

public:

    /// <summary>
    /// コンボ加算
    /// </summary>
    void AddCombo(float count);

private:

    /// <summary>
    /// ImGui
    /// </summary>
    void ImGui();

    /// <summary>
    /// Jsonに保存項目の追加
    /// </summary>
    void InitJsonBinder();


private:
    ComboManager() {}
    ~ComboManager() {}
    /*===============================================================//
                         　　 ポインタなど
    //===============================================================*/
	std::unique_ptr<Sprite> sprite_;
    std::unique_ptr<JsonBinder> jsonBinder_ ;



    float currentCombo_ = 0u;
	float saveMaxCombo_ = 0u;



};
