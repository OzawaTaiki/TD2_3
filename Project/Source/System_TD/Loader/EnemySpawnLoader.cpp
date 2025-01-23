#include "EnemySpawnLoader.h"
#include <fstream>
#include <sstream>
#include <cassert>

void EnemySpawnLoader::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/Data/EnemySpawn/EnemySpawnData_New.csv");
	assert(file.is_open());
    
    std::string line;
    Wave currentWave;
    Group currentGroup;
    bool inGroup = false;

    while (std::getline(file,line))
    {
        // 空白が合ったら削除
        line.erase(
            std::remove_if(
                line.begin(),
                line.end(),
                [](unsigned char c) { return std::isspace(c); } // キャストを追加
            ),
            line.end()
        );

        // 空行をスキップ
        if(line.empty()) continue;

        // コメントをスキップ
        if (line.find("//") == 0) continue;


        std::istringstream lineStream(line);
        std::string command;
        std::getline(lineStream, command, ',');

        if (command == "Wave") {
            // ウェーブの開始
            if (inGroup) {
                currentWave.groups.push_back(currentGroup);
                currentGroup = Group();
                inGroup = false;
            }
            if (currentWave.waveNumber != 0) {
                waves_.push_back(currentWave);
                currentWave = Wave();
            }
            std::string waveNumStr;
            std::getline(lineStream, waveNumStr, ',');
            currentWave.waveNumber = std::stoi(waveNumStr);
        }
        else if (command == "Group") {
            // グループの開始
            if (inGroup) {
                currentWave.groups.push_back(currentGroup);
                currentGroup = Group();
            }
            inGroup = true;
        }
        else if (command == "Num") {
            std::string numSre;
            std::getline(lineStream, numSre, ',');
            currentGroup.numEnemies = std::stoi(numSre);
        }
        else if (command == "Speed") {
            std::string speedStr;
            std::getline(lineStream, speedStr, ',');
            currentGroup.speed = std::stof(speedStr);

        }

        else if (command == "Goal") {
            std::string xStr, yStr, zStr;
            std::getline(lineStream, xStr, ',');
            std::getline(lineStream, yStr, ',');
            std::getline(lineStream, zStr, ',');
            currentGroup.goal = Vector3(std::stof(xStr), std::stof(yStr), std::stof(zStr));
        }
        else if (command == "Point") {
            std::string xStr, yStr, zStr;
            std::getline(lineStream, xStr, ',');
            std::getline(lineStream, yStr, ',');
            std::getline(lineStream, zStr, ',');
            currentGroup.spawnPoint = Vector3(std::stof(xStr), std::stof(yStr), std::stof(zStr));
        }
        else if (command == "Dir") {
            std::string xStr, yStr, zStr;
            std::getline(lineStream, xStr, ',');
            std::getline(lineStream, yStr, ',');
            std::getline(lineStream, zStr, ',');
            currentGroup.direction = Vector3(std::stof(xStr), std::stof(yStr), std::stof(zStr)).Normalize();
        }
        else if (command == "Offset") {
            std::string offsetStr;
            std::getline(lineStream, offsetStr, ',');
            currentGroup.offset = std::stof(offsetStr);
        }
        else if (command == "Wait") {
            std::string waitStr;
            std::getline(lineStream, waitStr, ',');
            currentGroup.waitTime = std::stoi(waitStr);
        }
        // 必要に応じて他のコマンドも処理
    }

    // 最後のグループとウェーブを追加
    if (inGroup) {
        currentWave.groups.push_back(currentGroup);
    }
    if (currentWave.waveNumber != 0) {
        waves_.push_back(currentWave);
    }

    file.close();

}

void EnemySpawnLoader::UpdateEnemyPopCommands()
{
    if (currentWaveIndex_ >= waves_.size()) {
        return;
    }

    if (isWait_) {
        waitTimer_--;
        if (waitTimer_ <= 0) {
            isWait_ = false;
        }
        return;
    }

    Wave& currentWate = waves_[currentWaveIndex_];
    if (currentGroupIndex_ >= currentWate.groups.size()) {
        // 現在のウェーブが完了したら次のウェーブへ
        currentWaveIndex_++;
        currentGroupIndex_ = 0;
        return;
    }

    Group& currentGroup = currentWate.groups[currentGroupIndex_];
    // 現在のグループの敵をスポーンさせる
    for (uint32_t i = 0; i < currentGroup.numEnemies; ++i) {
        // スポーン位置
        Vector3 position = currentGroup.spawnPoint + currentGroup.direction * currentGroup.offset * i;
        float speed = currentGroup.speed;
        Vector3 goal = currentGroup.goal;

        if (spawnCallback_) {
            spawnCallback_(position, speed, goal);
        }
    }

    if (currentGroup.waitTime > 0) {
        isWait_ = true;
        waitTimer_ = currentGroup.waitTime;
    }

    // 次のグループ
    currentGroupIndex_++;

}
