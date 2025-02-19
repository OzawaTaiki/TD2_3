#pragma once



#include <cstdint>
#include <memory>
#include <Systems/JsonBinder/JsonBinder.h>
#include <Rendering/Sprite/Sprite.h>

#include <vector>
#include <algorithm>
#include <chrono> 


class ComboManager
{
public:
    static ComboManager* GetInstance();


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
    /// コンボリセット
    /// </summary>
    void ResetCombo();

    /// <summary>
    /// コンボ加算
    /// </summary>
    void AddCombo(int count);

    /// <summary>
    /// 
    /// </summary>
    void EndGame();

    /// <summary>
    /// Top3スコアの更新
    /// </summary>
    void UpdateTopCombos();


    /// <summary>
    /// 現在のスコアを取得
    /// </summary>
    /// <returns></returns>
    int GetCurrentCombo() { return currentCombo_; }

	/// <summary>
	/// ゲーム中の最大コンボ数を取得
	/// </summary>
	/// <returns></returns>
	int GetMaxCombo() { return maxCombo_; }

    /// <summary>
    /// Top3スコアを取得
    /// </summary>
    /// <returns></returns>
    const std::vector<uint32_t>& GetTopCombos() const { return topCombos_; }

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
    ComboManager() = default;
    ~ComboManager() = default;
    ComboManager(const ComboManager&) = delete;
    ComboManager& operator=(const ComboManager&) = delete;
    /*===============================================================//
                         　　 ポインタなど
    //===============================================================*/
	Sprite* sprite_;
    std::unique_ptr<JsonBinder> jsonBinder_ ;



    int currentCombo_ = 0u;
	int saveMaxCombo_ = 0u;
    int maxCombo_ = 0; // ゲーム中の最大コンボ数
    std::vector<uint32_t> topCombos_ = { 0, 0, 0 };

    // 最後にコンボが加算された時間
    std::chrono::steady_clock::time_point lastComboTime_;
    const int comboResetTime_ = 5;

};
