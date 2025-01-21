#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <iosfwd>
#include <functional>


#include <Physics/Math/Vector3.h>

class EnemySpawnLoader {
public:

	using SpawnCallback = std::function<void(Vector3&)>;

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// コールバック登録
	/// </summary>
	/// <param name="callback"></param>
	void SetSpawnCallback(SpawnCallback callback) { spawnCallback_ = callback; }

private:

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isWait_ = false;
	int waitTimer_ = 0;

	SpawnCallback spawnCallback_ = nullptr;
};
