#pragma once

/// <summary>
/// スコア
/// </summary>
class ScoreManager
{


public:
	static ScoreManager* GetInstance();

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


	int currentScore_ = 0;

};

