#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <iosfwd>
#include <functional>
#include <fstream>

#include <Physics/Math/Vector3.h>


struct Group {
	int numEnemies;	 // 敵の数
	Vector3 velocity;		 // 速度
	Vector3 goal;			 // 目的地
	Vector3 spawnPoint;		 // スポーンする基準点
	Vector3 direction;		 // 列の向き
	float offset;			 // 敵同士の間隔
	int waitTime;			 // 待機時間
};

struct Wave {
	int waveNumber;			   // ウェーブ数
	std::vector<Group> groups; // グループのリスト
};

class EnemySpawnLoader {
public:

	using SpawnCallback = std::function<void(Vector3&,Vector3&,Vector3&)>;

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


	std::ifstream file_;
	std::vector<Wave> waves_;

	size_t currentWaveIndex_ = 0;
	size_t currentGroupIndex_ = 0;

	bool isWait_ = false;
	int waitTimer_ = 0;

	SpawnCallback spawnCallback_ = nullptr;
};
