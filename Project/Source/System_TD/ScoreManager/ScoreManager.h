#pragma once
#include <Systems/JsonBinder/JsonBinder.h>


class ScoreManager
{


public:
	static ScoreManager* GetInstance();

	/// <summary>
	/// /初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// スコア加算
	/// </summary>
	/// <param name="add"></param>
	void AddScore(int add);


	/// <summary>
	/// スコアをリセット
	/// </summary>
	void ResetScore();


	/// <summary>
	/// スコアの描画
	/// </summary>
	void DrawScore();


private:

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGui();

	/// <summary>
	/// JsonBinderの初期化
	/// </summary>
	void InitJsonBinder();

public:

	/// <summary>
	/// 現在のスコアを取得
	/// </summary>
	/// <returns></returns>
	int GetCurrentScore() { return currentScore_; }


private:

	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

	std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;


	int currentScore_ = 0;
	int saveMaxScore_ = 0;
};

