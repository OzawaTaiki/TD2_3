#include "GameScene.h"

#include <Framework/eScene/SceneManager.h>
#include <Rendering/Model/ModelManager.h>
#include <Rendering/Light/LightingSystem.h>
#include <Rendering/LineDrawer/LineDrawer.h>
#include "../System_TD/ScoreManager/ScoreManager.h"
#include "../System_TD/ComboManager/ComboManager.h"
#include "../System_TD/CountManager/CountManager.h"

#include <DirectXMath.h>
#include <ResourceManagement/TextureManager/TextureManager.h>

const std::string defaulFilPath = "Resources./Textures./";
std::unique_ptr<BaseScene>GameScene::Create()
{
    return std::make_unique<GameScene>();
}

GameScene::~GameScene()
{
    audio_->SoundStop(bgmVoice_);

    if (loadThread_ && loadThread_->joinable())
    {
        loadThread_->join();
    }

    delete loadScene_;
    delete area_;
}

void GameScene::Initialize()
{
    loadScene_ = new LoadScene();
    loadScene_->Initialize();
    Loading_ = true;

    loadThread_ = std::make_unique <std::thread>(&GameScene::Load, this);
    //loadThread_->detach();
}

void GameScene::Update()
{
    if (Loading_)
    {
        loadScene_->Update();
        return;
    }

    bool isCount = false;
    float preTime_ = std::ceilf(time_);

	float deltaTime = 1.0f / 60.0f;
	time_ += deltaTime;

    float curTime = std::floorf(time_);
    if (curTime == preTime_)
    {
        isCount = true;
    }

    if (time_ >= 55.0f && isCount)
    {
        audio_->SoundPlay(countHandle_, 1.0f, false, true);
    }


    //if (time_ >= 60.0f) {
    //    ScoreManager::GetInstance()->EndGame();
    //    ComboManager::GetInstance()->EndGame();
    //    SceneManager::GetInstance()->ReserveScene("Result");
    //    time_ = 0.0f;
    //}

    /*===============================================================//
                         　　    当たり判定
    //===============================================================*/
    collisionManager_->ResetColliderList();

#ifdef _DEBUG
    ImGui::Text("time: %f", time_);

    if (Input::GetInstance()->IsKeyTriggered(DIK_RETURN) &&
        Input::GetInstance()->IsKeyPressed(DIK_LSHIFT)) {
        enableDebugCamera_ = !enableDebugCamera_;
    }

    lightGroup_.DrawDebugWindow();
    ImGui();
#endif // _DEBUG
    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);

    if(player_->CanSwitchScene())
    {
		ScoreManager::GetInstance()->EndGame();
		ComboManager::GetInstance()->EndGame();
        SceneManager::GetInstance()->ReserveScene("Result");
    }

    player_->Update();
    enemyManager_->Update();
    area_->Update(player_.get());
    countDown_->Update();

	for (int i = 0; i < 10; ++i)
	{
        scoreSprites_[i]->Update();
	}

	CountManager::GetInstance()->ImGui();
    ScoreManager::GetInstance()->GameUpdate();
    ComboManager::GetInstance()->Update();
    comboCount_ = ComboManager::GetInstance()->GetCurrentCombo();
    scoreCount_ = ScoreManager::GetInstance()->GetCurrentScore();

    UpdateEnemyScore();
    if (enableDebugCamera_)
    {
        debugCamera_.Update();
        SceneCamera_.matView_ = debugCamera_.matView_;
        SceneCamera_.TransferData();
    }
    else
    {
        followCamera_.Update(!player_->IsAlive());
        SceneCamera_.matView_ = followCamera_.matView_;
        SceneCamera_.translate_ = followCamera_.translate_;
        SceneCamera_.rotate_ = followCamera_.rotate_;
        SceneCamera_.Update();
        SceneCamera_.UpdateMatrix();
    }

    collisionManager_->CheckAllCollision();
}

void GameScene::Draw()
{
    if (Loading_)
    {
        loadScene_->Draw();
        return;
    }

    Sprite::PreDraw();
    for (int i = 0; i < bg_; ++i)
    {
        backGrounds_[i]->Draw();
    }

    ModelManager::GetInstance()->PreDrawForObjectModel();
    area_->Draw(&SceneCamera_);

    player_->Draw({ 1,1,1,1 });
    enemyManager_->Draw({ 1,1,1,1 });

    LineDrawer::GetInstance()->Draw();

    Sprite::PreDraw();
	for (int i = 0; i < bg_; ++i)
	{
		backGrounds_[i]->Draw();
	}
	DrawScore();
    countDown_->Draw();
	//DrawCombo();
	DrawEnemyScore();
	player_->DrawSprite();
}

void GameScene::AddEnemyScore(int score)
{
    Score newScore;
    newScore.lifetime = scorePopupDuration_;
    newScore.position = { 1180, 100.0f + static_cast<float>(scores_.size()) * 25.0f }; // 表示位置をずらす

    std::string scoreStr = "+" + std::to_string(score);
    float x = newScore.position.x;

    // 各文字ごとの間隔を設定
    std::unordered_map<char, float> digitSpacing = {
        {'+', 30.0f}, // + は 15px の間隔
        {'0', 23.0f}, {'1', 20.0f}, {'2', 20.0f}, {'3', 20.0f},
        {'4', 20.0f}, {'5', 20.0f}, {'6', 20.0f}, {'7', 20.0f},
        {'8', 20.0f}, {'9', 20.0f}
    };

    for (char c : scoreStr)
    {
        int digitIndex = (c == '+') ? 0 : (c - '0' + 1); // UVの位置

        // 文字ごとの間隔を取得（見つからなければデフォルト 20.0f）
        float digitWidth = digitSpacing.count(c) ? digitSpacing[c] : 20.0f;

        std::unique_ptr<Sprite> digitSprite(Sprite::Create(scoreTexture_));
        digitSprite->Initialize();
        digitSprite->uvScale_ = { 0.1f, 1.0f };
        digitSprite->uvTranslate_ = { 0.1f * digitIndex, 0.0f };
        digitSprite->translate_ = { x, newScore.position.y };
        digitSprite->scale_ = { 0.07f, 0.6f };

        newScore.digits.push_back(std::move(digitSprite));

        x += digitWidth; // 文字ごとの間隔を反映
    }

    scores_.push_back(std::move(newScore));
}

void GameScene::Load()
{
    SceneCamera_.Initialize();
    SceneCamera_.translate_ = { 0,100,0 };
    SceneCamera_.rotate_ = { DirectX::XMConvertToRadians(90.0f),0,0 };
    SceneCamera_.UpdateMatrix();
    debugCamera_.Initialize();
    followCamera_.Initialize();

    collisionManager_ = CollisionManager::GetInstance();

    player_ = std::make_unique<Player>();
    player_->Initialize(&SceneCamera_);
    followCamera_.SetTarget(player_->GetWorldTransform());

    enemyManager_ = std::make_unique<EnemyManager>();
    enemyManager_->SetPlayer(player_.get());
    enemyManager_->Initialize(&SceneCamera_);
	enemyManager_->SetGameScene(this);

    audio_ = AudioSystem::GetInstance();

    bgmHandle_ = audio_->SoundLoadWave("bgm.wav");

    countHandle_ = audio_->SoundLoadWave("count.wav");


    lightGroup_.Initialize();
    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);

    LineDrawer::GetInstance()->SetCameraPtr(&SceneCamera_);


    area_ = new Area();
    area_->Initialize(50);

    DirectionalLight DLight{};
    DLight.direction = Vector3(-0.909f, -0.417f, 0.0f).Normalize();
    DLight.intensity = 0.2f;

    PointLight PLight{};
    PLight.position = Vector3(0, -0.1f, 0);
    PLight.intensity = 8.0f;
    PLight.radius = 20.0f;
    PLight.decay = 2.0f;

    lightGroup_.SetDirectionalLight(DLight);
    lightGroup_.AddPointLight(PLight, "Player", player_->GetWorldPositionRef());

    LightingSystem::GetInstance()->SetLightGroup(&lightGroup_);

    /*===============================================================//
                 　　             スコア関連
    //===============================================================*/
    ComboManager::GetInstance()->Initialize();
	ScoreManager::GetInstance()->Initialize();
	CountManager::GetInstance()->Initialize();

    /*===============================================================//
                 　　             スプライト
    //===============================================================*/
    uint32_t bg[bg_] = { TextureManager::GetInstance()->Load("frame.png", defaulFilPath),
                         TextureManager::GetInstance()->Load("player.png", defaulFilPath) };
    backGrounds_[0] = Sprite::Create(bg[0]);
	backGrounds_[0]->Initialize();
	backGrounds_[0]->translate_ = { -9,-9 };
	backGrounds_[0]->scale_ = { 1.018f,1.03f };
	backGrounds_[0]->SetAnchor({ 0.0f,0.0f });

    backGrounds_[1] = Sprite::Create(bg[1]);
    backGrounds_[1]->Initialize();
    backGrounds_[1]->SetAnchor({ 0.5f,0.5f });
    backGrounds_[1]->translate_ = { 1100,515 };
	//backGrounds_[1]->rotate_ = 44.5f;


    ////// -----------------------------------
    //////              スコア
    ////// -----------------------------------
    uint32_t scoreNumber = TextureManager::GetInstance()->Load("number.png", defaulFilPath);

    for (int i = 0; i < 10; ++i) {
        scoreSprites_[i] = Sprite::Create(scoreNumber);
        scoreSprites_[i]->Initialize();
        scoreSprites_[i]->translate_ = {  };
        scoreSprites_[i]->scale_ = { 0.07f, 0.6f };
        scoreSprites_[i]->uvScale_ = { 0.1f,1.0f };
        scoreSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
    }

    scoreTexture_ = TextureManager::GetInstance()->Load("number_2.png", defaulFilPath);

	//for (int i = 0; i < 11; ++i)
	//{
	//	scoreSprites_[i] = Sprite::Create(scoreTexture_);
	//	scoreSprites_[i]->Initialize();
 //       scoreSprites_[i]->scale_ = { 0.07f, 0.5f };
 //       scoreSprites_[i]->uvScale_ = { 0.1f,1.0f };
 //       scoreSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	//}



    ////// -----------------------------------
    //////              コンボ
    ////// -----------------------------------
    uint32_t comboNumber = TextureManager::GetInstance()->Load("number_2.png", defaulFilPath);

    for (int i = 0; i < 11; ++i) {
        comboSprites_[i] = Sprite::Create(comboNumber);
        comboSprites_[i]->Initialize();
        comboSprites_[i]->translate_ = {  };
        comboSprites_[i]->scale_ = { 0.07f, 0.5f };
        comboSprites_[i]->uvScale_ = { 0.1f,1.0f };
        comboSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
    }

    countDown_ = std::make_unique<CountDown>();
    countDown_->Initialize(60, { 30,5,4,3,2,1 });

	gameTime_ = GameTime::GetInstance();
    gameTime_->CreateChannel("GameScene");

    Loading_ = false;

    bgmVoice_ = audio_->SoundPlay(bgmHandle_, 1.0f, true);
}

void GameScene::DrawScore()
{
    std::string scoreStr = std::to_string(scoreCount_); // スコアを文字列に変換
    while (scoreStr.size() < 8) {
        scoreStr = "0" + scoreStr; // 6桁に満たない場合は左側をゼロで埋める
    }

    float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
    float x = 1280.0f - (scoreStr.size() * digitWidth); // スコア全体の右端を基準に調整
    float y = 50.0f; // スコアの描画位置（固定）

    // スコアの桁数に応じてスプライトを更新
    for (size_t i = 0; i < scoreStr.size(); ++i) {
        int digit = scoreStr[i] - '0'; // 数字に変換
        if (digit < 0 || digit > 9) continue;

        auto& sprite = scoreSprites_[i]; // 既存スプライトを使い回す
        sprite->uvTranslate_ = { digit * 0.1f, 0.0f };
        sprite->translate_ = { x, y };

        // 描画
        sprite->Draw();

        x += digitWidth; // 次の桁の位置に移動
    }
}



void GameScene::DrawCombo()
{
    std::string scoreStr = std::to_string(comboCount_); // スコアを文字列に変換
    float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
    float x = 700.0f - (scoreStr.size() * digitWidth); // スコア全体の右端を基準に調整
    float y = 530.0f; // スコアの描画位置（固定）

    // スコアの桁数に応じてスプライトを更新
    for (size_t i = 0; i < scoreStr.size(); ++i) {
        int digit = scoreStr[i] - '0'; // 数字に変換
        if (digit < 0 || digit > 9) continue;

        auto& sprite = comboSprites_[i]; // 既存スプライトを使い回す
        sprite->uvTranslate_ = { digit * 0.1f, 0.0f };
        sprite->translate_ = { x, y };

        // 描画
        sprite->Draw();

        x += digitWidth; // 次の桁の位置に移動
    }
}

void GameScene::UpdateEnemyScore()
{
    float deltaTime = 1.0f / 60.0f;

    for (auto& score : scores_)
    {
        score.lifetime -= deltaTime;
        if (score.lifetime <= 0.5f)
        {
            for (auto& digit : score.digits)
            {
                digit->translate_.x += 100.0f * deltaTime; // 右に移動
				digit->SetColor({ 1.0f, 1.0f, 1.0f, score.lifetime * 2.0f }); // 徐々に透明に
            }
        }
    }

    // 一番古いスコアを削除する
    auto it = std::remove_if(scores_.begin(), scores_.end(),
        [](const Score& s) { return s.lifetime <= 0.0f; });

    bool removed = (it != scores_.end());

    // 消えたスコアを削除
    scores_.erase(it, scores_.end());

    // **削除後に位置を詰める**
    if (removed)
    {
        float baseY = 100.0f; // 一番上のスコアのY座標
        float offsetY = 25.0f; // 1つ下のスコアのオフセット

        // 残っているスコアを詰める
        for (size_t i = 0; i < scores_.size(); ++i)
        {
            scores_[i].position.y = baseY + i * offsetY;

            for (auto& digit : scores_[i].digits)
            {
                digit->translate_.y = scores_[i].position.y;
            }
        }
    }
}

void GameScene::DrawEnemyScore()
{
    for (auto& score : scores_)
    {
        for (auto& digit : score.digits)
        {
            digit->Draw();
        }
    }
}

#ifdef _DEBUG
#include <imgui.h>
void GameScene::ImGui()
{
	ImGui::Begin("GameScene");
    ImGui::DragFloat3("scale", &backGrounds_[0]->scale_.x);
	ImGui::DragFloat3("translate", &backGrounds_[0]->translate_.x);

    ImGui::DragFloat("Time", &time_);
	ImGui::End();
}
#endif // _DEBUG