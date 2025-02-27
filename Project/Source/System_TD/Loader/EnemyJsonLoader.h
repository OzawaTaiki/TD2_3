#pragma once
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <json.hpp>
#include <imgui.h> // ImGui を追加
#include <Rendering/Sprite/Sprite.h>
#include <Physics/Math/Vector3.h>

struct Group {
    int numEnemies;
    std::string     moveType = "Default";
    float           speed;
    Vector3         goal;
    Vector3         spawnPoint;
    Vector3         direction;
    float           offset;
    float           waitTime;
    //bool            isWarning = false;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Group, numEnemies, moveType, speed, goal, spawnPoint, direction, offset, waitTime)
};


struct Wave {
    int waveNumber;
    std::vector<Group> groups;
    float waitTime;
    float waveTimer;     
    bool isWaveActive; 

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Wave, waveNumber, groups, waitTime, waveTimer, isWaveActive)
};

struct SpawnView {
    Vector3 position;
    float timer;
    Sprite* sprite;
};

struct SpawnInfo {
    Vector3 position;
    float speed;
    Vector3 goal;
    std::string moveType;
};


class Player;
class EnemyJsonLoader {
public:
    using SpawnCallback = std::function<void(Vector3&, float&, Vector3&, std::string&)>;
    using WarningCallback = std::function<void(Vector3& position, float warningTime)>;
   

    /// <summary>
    /// 敵発生データの読み込み
    /// </summary>
    void LoadEnemyPopData();

    /// <summary>
    /// 敵発生データの保存
    /// </summary>
    void SaveEnemyPopData() const;

    /// <summary>
    /// 敵発生コマンドの更新
    /// </summary>
    void UpdateEnemyPopCommands();


    /// <summary>
	/// 全てのウェーブを更新
    /// </summary>
    void UpdateAllWaves();

    /// <summary>
    /// ImGui の UI を表示
    /// </summary>
    void ShowImGui();

    /// <summary>
    /// コールバック登録
    /// </summary>
    void SetSpawnCallback(SpawnCallback callback) { spawnCallback_ = callback; }
    void SetWarningCallback(WarningCallback callback) { warningCallback_ = callback; }


public:

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

    float GetSpawnWarningTime() { return kSpawnWarningTime_; }
private:

    /// <summary>
    /// ウェーブ全体の更新
    /// </summary>
    void UpdateWave();

    /// <summary>
    /// グループ全体の更新
    /// </summary>
    void UpdateGroup();

    /// <summary>
    /// 特定のウェーブ、グループの更新7
    /// </summary>
    /// <param name="waveIndex"></param>
    void IUpdateWave(size_t waveIndex);
    void IUpdateGroup(size_t waveIndex, size_t groupIndex);

    /// <summary>
    /// プレイヤーの位置をリアルタイムで更新する
    /// </summary>
    void UpdateEnemyPositions();

private:
	Player* player_ = nullptr;
    std::vector<Wave> waves_;
	std::unordered_map<int, float> originalSpeeds_;
    size_t currentWaveIndex_ = 0;
    size_t currentGroupIndex_ = 0;
    bool isWait_ = false;
    float waitTimer_ = 0.0f;
    bool isUpdate_ = true;

    float currentTime_ = 0.0f;   
    float waveStartTime_ = 0.0f;
    bool isFirstWave_ = true;    
    bool hasWarned_ = false;

    SpawnCallback spawnCallback_ = nullptr;
    WarningCallback warningCallback_ = nullptr;
    SpawnInfo nextSpawnInfo_;
    float kSpawnWarningTime_ = 2.0f;
};
