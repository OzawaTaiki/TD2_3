#pragma once
#include <Systems/JsonBinder/JsonBinder.h>


#include <vector>
#include <algorithm>

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

	/// <summary>
	/// 
	/// </summary>
	void EndGame();

	/// <summary>
	/// Top3スコアの更新
	/// </summary>
	void UpdateTopScores();
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


	/// <summary>
	/// Top3スコアを取得
	/// </summary>
	/// <returns></returns>
	const std::vector<uint32_t>& GetTopScores() const { return topScores_; }

private:

	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

	std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;


	int currentScore_ = 0;
	int saveMaxScore_ = 0;
	std::vector<uint32_t> topScores_ = { 0, 0, 0 };
};

