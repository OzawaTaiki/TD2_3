#pragma once
#include <cstdint>


class CountManager
{

public:


	static CountManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGui();

	/// <summary>
	/// 敵を倒した数をカウント
	/// </summary>
	void EnemyCount(uint32_t count);

private:






public:

	/// <summary>
	/// 敵を倒した数をカウント
	/// </summary>
	/// <param name="count"></param>
	void SetEnemyCount(uint32_t count) { EnemyCount(count); }

	/// <summary>
	/// 敵を倒した数を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetEnemyCount() { return enemyCount_; }



private:

	CountManager() = default;
	~CountManager() = default;
	CountManager(const CountManager&) = delete;
	CountManager& operator=(const CountManager&) = delete;

	uint32_t enemyCount_ = 0;


};

