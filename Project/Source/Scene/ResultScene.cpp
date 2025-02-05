#include "ResultScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>
#include <ResourceManagement/TextureManager/TextureManager.h>
#include<Systems/Utility/RandomGenerator.h>

const std::string defaulFilPath = "Resources./Textures./";

std::unique_ptr<BaseScene> ResultScene::Create()
{
	return std::make_unique<ResultScene>();
}

ResultScene::~ResultScene()
{
    audio_->SoundStop(bgmVoice_);
    audio_->SoundStop(drumrollVoice_);
    audio_->SoundStop(drumrollEndVoice_);
    audio_->SoundStop(buttonVoice_);

}

void ResultScene::Initialize()
{
	uint32_t th[ms_] = { TextureManager::GetInstance()->Load("backGround.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("frame.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("result.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("player.png", defaulFilPath) };

    resultSprite_[0] = Sprite::Create(th[0]);
	resultSprite_[0]->Initialize();
	resultSprite_[0]->SetAnchor({ 0.0f,0.0f });
	resultSprite_[0]->SetSize({ 1280,720 });
	resultSprite_[0]->SetColor({0.15,0.15,0.15,1.0});

    resultSprite_[1] = Sprite::Create(th[1]);
	resultSprite_[1]->Initialize();
	resultSprite_[1]->translate_ = { -9,-9 };
	resultSprite_[1]->scale_ = { 1.018f,1.03f };
	resultSprite_[1]->SetAnchor({ 0.0f,0.0f });

    resultSprite_[2] = Sprite::Create(th[2]);
	resultSprite_[2]->Initialize();
	resultSprite_[2]->SetAnchor({ 0.5f,0.5f });
	resultSprite_[2]->translate_ = { 540,360 };

    resultSprite_[3] = Sprite::Create(th[3]);
	resultSprite_[3]->Initialize();
	resultSprite_[3]->SetAnchor({ 0.5f,0.5f });
	resultSprite_[3]->translate_ = { 180,550 };
	resultSprite_[3]->rotate_ = 44.3f;

	/*===============================================================//
			 　　				スコア
	//===============================================================*/
	uint32_t scoreTh = TextureManager::GetInstance()->Load("number.png", defaulFilPath);
	targetScore_ = ScoreManager::GetInstance()->GetCurrentScore();
	for (int i = 0; i < 10; ++i) {

        scoreSprites_[i] = Sprite::Create(scoreTh, { 0,0 });
		scoreSprites_[i]->Initialize();
		scoreSprites_[i]->translate_ = {  };
		scoreSprites_[i]->scale_ = { 0.07f, 0.5f };
		scoreSprites_[i]->uvScale_ = { 0.1f,1.0f };
		scoreSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	}


	/*===============================================================//
		 　　					コンボ
	//===============================================================*/
	targetCombo_ = ComboManager::GetInstance()->GetMaxCombo();
	for (int i = 0; i < 10; ++i) {

		comboSprites_[i] = Sprite::Create(scoreTh, { 0,0 });
		comboSprites_[i]->Initialize();
		comboSprites_[i]->translate_ = {  };
		comboSprites_[i]->scale_ = { 0.07f, 0.5f };
		comboSprites_[i]->uvScale_ = { 0.1f,1.0f };
		comboSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	}


	/*===============================================================//
	 　　						敵倒
	//===============================================================*/

	targetCount_ = CountManager::GetInstance()->GetEnemyCount();
	for (int i = 0; i < 10; ++i) {
        countSprites_[i] = Sprite::Create(scoreTh, { 0,0 });
		countSprites_[i]->Initialize();
		countSprites_[i]->translate_ = {  };
		countSprites_[i]->scale_ = { 0.07f, 0.5f };
		countSprites_[i]->uvScale_ = { 0.1f,1.0f };
		countSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	}

	/*===============================================================//
 　　							UI
	//===============================================================*/
	uiTitle_.Initialize("ToTitle");
	uiA_.Initialize("AButton");

	UI_TX[0] = { TextureManager::GetInstance()->Load("resultTitle.png", defaulFilPath) };
	UI_TX[1] = { TextureManager::GetInstance()->Load("resultRetry.png", defaulFilPath) };
	uiTitle_.SetTextureHandle(UI_TX[0]);

    audio_ = AudioSystem::GetInstance();

    drumrollHandle_ = audio_->SoundLoadWave("drumroll.wav");
    //drumrollEndHandle_ = audio_->SoundLoadWave("drumrollEnd.wav");
    buttonHandle_ = audio_->SoundLoadWave("button.wav");

    bgmHandle_ = audio_->SoundLoadWave("result.wav");
	bgmVoice_ = audio_->SoundPlay(bgmHandle_, 1.0f, true);

	isRetry_ = true;
}

void ResultScene::Update()
{
	static float vol = 1.0f;
#ifdef _DEBUG


	ImGui::Begin("ResultScene");
	ImGui::DragFloat("Score", &resultSprite_[3]->rotate_);

    ImGui::DragFloat("Volume", &vol, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Start", &buttonOffset, 0.01f, 0.0f);
	if (ImGui::Button("sound"))
	{
		audio_->SoundPlay(buttonHandle_, vol, false, false, buttonOffset);
	}

	ImGui::End();

#endif // _DEBUG


	ScoreManager::GetInstance()->Update();
	ComboManager::GetInstance()->Update();
	CountManager::GetInstance()->ImGui();

	uiTitle_.Update();
	uiA_.Update();

	if (drawSelect_)
	{
		// コントローラーの左スティックを取得
		Vector2 leftStick = Input::GetInstance()->GetPadLeftStick();

		// 上下の入力で切り替え
		if (std::abs(leftStick.y) > 0.5f)
		{
			if (leftStick.y > 0.0f && isRetry_)  // 上に倒したとき
			{
				buttonVoice_ = audio_->SoundPlay(buttonHandle_, vol, false, true, buttonOffset);
				isRetry_ = false;
			}
			else if (leftStick.y < 0.0f && !isRetry_)  // 下に倒したとき
			{
				buttonVoice_ = audio_->SoundPlay(buttonHandle_, vol, false, true, buttonOffset);
				isRetry_ = true;
			}
		}

		if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_Down) ||
			Input::GetInstance()->IsPadTriggered(PadButton::iPad_Up)) {
			buttonVoice_ = audio_->SoundPlay(buttonHandle_, vol, false, true, buttonOffset);

			isRetry_ = !isRetry_;
		}


		// 既存のパッドボタン入力処理...
		if (!isRetry_) {
			uiTitle_.SetTextureHandle(UI_TX[1]);
			if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A)) {
				buttonVoice_ = audio_->SoundPlay(buttonHandle_, vol, false, true, buttonOffset);
				SceneManager::GetInstance()->ReserveScene("Game");
			}
		}
		else {
			uiTitle_.SetTextureHandle(UI_TX[0]);
			if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A)) {
				buttonVoice_ = audio_->SoundPlay(buttonHandle_, vol, false, true, buttonOffset);
				SceneManager::GetInstance()->ReserveScene("Title");
			}
		}

	}
    UpdateNumbers();
}

void ResultScene::Draw()
{
	Sprite::PreDraw();
	for (uint32_t i = 0; i < ms_; ++i)
	{
		if(i == 0 || i == 2){
			resultSprite_[i]->Draw();
		}

	}

	DrawScore();
	DrawCombo();
	DrawCountEnemy();

	if (drawSelect_)
	{
		uiTitle_.Draw();
		uiA_.Draw();
	}
}

void ResultScene::DrawScore()
{
	std::string scoreStr = std::to_string(score_); // スコアを文字列に変換
	while (scoreStr.size() < 6) {
		scoreStr = "0" + scoreStr;
	}

	float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
	float x = 540.0f - (scoreStr.size() / 2.0f *digitWidth); // スコア全体の中心を基準に調整
	float y = 270.0f; // スコアの描画位置（固定）

	// スコアの桁数に応じてスプライトを更新
	for (size_t i = 0; i < scoreStr.size(); ++i) {
		int digit = scoreStr[i] - '0'; // 数字に変換
		if (digit < 0 || digit > 9) continue;

		auto& sprite = scoreSprites_[i]; // 既存スプライトを使い回す
		sprite->uvTranslate_ = { digit * 0.1f, 0.0f };
		sprite->translate_ = { x, y };

		// 描画
		Sprite::PreDraw();
		sprite->Draw();

		x += digitWidth; // 次の桁の位置に移動
	}
}

void ResultScene::DrawCombo()
{
	std::string scoreStr = std::to_string(combo_); // スコアを文字列に変換
	while (scoreStr.size() < 3) {
		scoreStr = "0" + scoreStr;
	}
	float base = static_cast<float>(scoreStr.size()) / 2.0f;
	float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
	float x = 540.0f - (base *digitWidth); // スコア全体の右端を基準に調整
	float y = 530.0f; // スコアの描画位置（固定）

	// スコアの桁数に応じてスプライトを更新
	for (size_t i = 0; i < scoreStr.size(); ++i) {
		int digit = scoreStr[i] - '0'; // 数字に変換
		if (digit < 0 || digit > 9) continue;

		auto& sprite = comboSprites_[i]; // 既存スプライトを使い回す
		sprite->uvTranslate_ = { digit * 0.1f, 0.0f };
		sprite->translate_ = { x, y };

		// 描画
		Sprite::PreDraw();
		sprite->Draw();

		x += digitWidth; // 次の桁の位置に移動
	}
}

void ResultScene::DrawCountEnemy()
{
	std::string scoreStr = std::to_string(count_); // スコアを文字列に変換
	while (scoreStr.size() < 3) {
		scoreStr = "0" + scoreStr;
	}

	float base = static_cast<float>(scoreStr.size()) / 2.0f;

	float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
	float x = 540 - (base *digitWidth); // スコア全体の右端を基準に調整
	float y = 410.0f; // スコアの描画位置（固定）

	// スコアの桁数に応じてスプライトを更新
	for (size_t i = 0; i < scoreStr.size(); ++i) {
		int digit = scoreStr[i] - '0'; // 数字に変換
		if (digit < 0 || digit > 9) continue;

		auto& sprite = countSprites_[i]; // 既存スプライトを使い回す
		sprite->uvTranslate_ = { digit * 0.1f, 0.0f };
		sprite->translate_ = { x, y };

		// 描画
		Sprite::PreDraw();
		sprite->Draw();

		x += digitWidth; // 次の桁の位置に移動
	}
}

void ResultScene::UpdateNumbers()
{
    if (drawSelect_)
        return;

    duration_ += GameTime::GetUnScaleDeltaTime_float();

	if (combo_ == targetCombo_)
	{
		if (duration_ >= 1.5f)
		{
			drawSelect_ = true;
		}
		return;
	}

	if (duration_ > timePerDigit_)
	{
        duration_ = 0.0f;
        currentDigitIndex++;
	}

    if (score_ != targetScore_)
    {
		score_ = RandomGenerator::GetInstance()->GetUniformInt(0, 1000000 - 1);

		// 桁数
		int numDigits = 1;

        if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A)) {
			currentDigitIndex++;
        }

		for (int i = 0; i < currentDigitIndex; ++i)
            numDigits *= 10;


        int num = targetScore_ % numDigits;
        score_ /= numDigits;
        score_ *= numDigits;
        score_ += num;

        if (numDigits >= targetScore_)
        {
            score_ = targetScore_;
            currentDigitIndex = 0;
            duration_ = 0.0f;
        }
    }
	else if (count_ != targetCount_)
	{
		count_ = RandomGenerator::GetInstance()->GetUniformInt(0, 1000 - 1);

		// 桁数
		int numDigits = 1;
		if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A)) {
			currentDigitIndex++;
		}
		for (int i = 0; i < currentDigitIndex; ++i)
			numDigits *= 10;

		int num = targetCount_ % numDigits;

		count_ /= numDigits;
		count_ *= numDigits;
		count_ += num;

		if (numDigits >= targetCount_)
		{
			count_ = targetCount_;
			currentDigitIndex = 0;
			duration_ = 0.0f;
		}
	}
    else if (combo_ != targetCombo_)
    {
        combo_ = RandomGenerator::GetInstance()->GetUniformInt(0, 1000 - 1);

        // 桁数
        int numDigits = 1;
		if (Input::GetInstance()->IsPadTriggered(PadButton::iPad_A)) {
			currentDigitIndex++;
		}
        for (int i = 0; i < currentDigitIndex; ++i)
            numDigits *= 10;

        int num = targetCombo_ % numDigits;
        combo_ /= numDigits;
        combo_ *= numDigits;
        combo_ += num;

        if (numDigits >= targetCombo_)
        {
            combo_ = targetCombo_;
            currentDigitIndex = 0;
            duration_ = 0.0f;
        }
    }
}


