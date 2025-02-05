#pragma once
#include <list>
#include <memory>
#include "Enemy.h"
#include "TitleEnemy.h"
#include <Framework/Camera/Camera.h>

#include <chrono>

#include "../../../System_TD/Loader/EnemySpawnLoader.h"
#include "../../../System_TD/Loader/EnemyJsonLoader.h"

#include <Systems/Time/GameTime.h>
#include <Systems/Audio/AudioSystem.h>
#include <Systems/JsonBinder/JsonBinder.h>

class GameScene;
class Player;
class EnemyManager
{
public:


    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Camera* camera);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw(const Vector4& color);



    bool AttractEnemy(std::list <std::unique_ptr<TitleEnemy>>& _enemies, float range = 10.0f) const;


private:

    /// <summary>
    /// ImGui
    /// </summary>
    void ImGui();

    /// <summary>
    /// タイムでスポーン
    /// </summary>
    void SpawnEnemy(Vector3& position,float& speed,Vector3& goal,std::string& moveType);

	/// <summary>
	/// スポーンの警告
	/// </summary>
	/// <param name="position"></param>
	/// <param name="warningTime"></param>
	void SpawnWarning(Vector3& position, float& warningTime);

    /// <summary>
    /// 敵を削除（デスフラグ付き）
    /// </summary>
    void RemoveDeadEnemies();

    /// <summary>
    /// 一定範囲内でランダム生成
    /// </summary>
    /// <returns></returns>
    Vector3 GenerateRandomPosition();


    /// <summary>
    ///
    /// </summary>
    /// <param name="range"></param>
    void AttractEnemy(float range);

    /// <summary>
    /// マークした敵を消滅させる
    /// </summary>
    void RemoveMarkedEnemies();

    Vector4 Vector4ooooo(const Matrix4x4& m, const Vector4& v);

    void InitJsonBinder();

public:

    void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
private:
    /*===============================================================//
                         構造体
    //===============================================================*/
    struct SpawnWarningModel {
        std::unique_ptr<ObjectModel> oModel;
        float timer;
        Vector3 position;
    };
    std::list<SpawnWarningModel> warningModels_;

    /*===============================================================//
                              ポインタ
    //===============================================================*/
    Player* player_;
    GameScene* gameScene_;
    uint32_t enemyCount_ = 3;
    std::list<std::unique_ptr<Enemy>> enemies_;
    Camera* camera_ = nullptr;
    GameTime* gameTime_ = nullptr;
    std::unique_ptr<JsonBinder> jsonBinder_ = nullptr;
    EnemyJsonLoader spawnJson_; 

    /*===============================================================//
                             ランダム生成
    //===============================================================*/

    float randomRangeXMin_ = -10.0f;
    float randomRangeXMax_ = 10.0f;
    float randomRangeZMin_ = -10.0f;
    float randomRangeZMax_ = 10.0f;

    /*===============================================================//
                                引き寄せ処理
    //===============================================================*/
    // 引き寄せ処理
    float attractSpeed_ = 0.1f;
    float attractRadius_ = 10.0f;

    float repelCoefficient_ = 10.0f;    // 反発の基準係数
    float attractCoefficient_ = 10.0f;  // 引き寄せの基準係数
    float maxRepelForce_ = 3.0f;        // 反発力の最大値
    float maxAttractForce_ = 3.0f;      // 引き寄せ力の最大値
    float threshold_ = 2.0f;            // 消滅のしきい値



    /*===============================================================//
                             敵の消滅させる関連
    //===============================================================*/

    float kSizeThreshold = 2.0f; // 敵を消滅させるOBBのサイズ x

    // 消滅させる敵のリスト
    std::list<std::list<std::unique_ptr<Enemy>>::iterator> markedForRemovalEnemies_;

    float minX_;
    float maxX_;
    float minY_;
    float maxY_;
    float minZ_;
    float maxZ_;
    float rotateAngle_ = 0.0f;

    // 時間経過でNone状態に戻す



    /*===============================================================//
                             スポーン関連
    //===============================================================*/

    std::chrono::steady_clock::time_point lastSpawnTime_; // 最後に敵を生成した時刻
    float spawnInterval_ = 5.0f; // 敵生成間隔
    EnemySpawnLoader spawnLoader_;


    /*===============================================================//
                             サウンド
    //===============================================================*/

    uint32_t hitHandle_;
    float hitVolume_ = 1.0f;
    float hitStartOffset_ = 0.0f;

    uint32_t deathHandle_;
    float deathVolume_ = 1.0f;
    float deathStartOffset_ = 0.0f;


    Vector3 warningRotate_ = { -90.0f, 0.0f, 180.0f };    // 回転速度
    Vector4 warningColor = { 1.0f,0.0f,0.0f,0.0f };
	Vector4 setColor = { 1.0f,0.0f,0.0f,1.0f };
    float warningRiseSpeed_ = 5.0f;       // 上昇速度
	float warningScaleSpeed_ = 5.0f;      // 拡大速度

};
