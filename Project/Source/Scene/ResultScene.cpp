#include "ResultScene.h"

#include <Systems/Input/Input.h>
#include <Framework/eScene/SceneManager.h>
#include <ResourceManagement/TextureManager/TextureManager.h>

const std::string defaulFilPath = "Resources./Textures./";

std::unique_ptr<BaseScene> ResultScene::Create()
{
	return std::make_unique<ResultScene>();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	uint32_t th[ms_] = { TextureManager::GetInstance()->Load("backGround.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("frame.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("result.png", defaulFilPath),
						 TextureManager::GetInstance()->Load("player.png", defaulFilPath) };
	
	resultSprite_[0] = std::make_unique<Sprite>();
	resultSprite_[0].reset(Sprite::Create(th[0]));
	resultSprite_[0]->Initialize();
	resultSprite_[0]->SetAnchor({ 0.0f,0.0f });
	//resultSprite_[0]->translate_ = {640,320};
	resultSprite_[0]->SetSize({ 1280,720 });
	resultSprite_[0]->SetColor({0.25,0.25,0.25,1.0});

	resultSprite_[1] = std::make_unique<Sprite>();
	resultSprite_[1].reset(Sprite::Create(th[1]));
	resultSprite_[1]->Initialize();
	resultSprite_[1]->translate_ = { -9,-9 };
	resultSprite_[1]->scale_ = { 1.018f,1.03f };
	resultSprite_[1]->SetAnchor({ 0.0f,0.0f });

	resultSprite_[2] = std::make_unique<Sprite>();
	resultSprite_[2].reset(Sprite::Create(th[2]));
	resultSprite_[2]->Initialize();
	resultSprite_[2]->SetAnchor({ 0.5f,0.5f });
	resultSprite_[2]->translate_ = { 640,360 };

	resultSprite_[3] = std::make_unique<Sprite>();
	resultSprite_[3].reset(Sprite::Create(th[3]));
	resultSprite_[3]->Initialize();
	resultSprite_[3]->SetAnchor({ 0.5f,0.5f });
	resultSprite_[3]->translate_ = { 150,600 };
	resultSprite_[3]->rotate_ = 44.5f;
	
	/*===============================================================//
			 　　				スコア
	//===============================================================*/
	uint32_t scoreTh = TextureManager::GetInstance()->Load("number.png", defaulFilPath);
	score_ = ScoreManager::GetInstance()->GetCurrentScore();
	for (int i = 0; i < 10; ++i) {
		scoreSprites_[i] = std::make_unique<Sprite>();
		scoreSprites_[i].reset(Sprite::Create(scoreTh));
		scoreSprites_[i]->Initialize();
		scoreSprites_[i]->translate_ = {  };
		scoreSprites_[i]->scale_ = { 0.07f, 0.5f }; 
		scoreSprites_[i]->uvScale_ = { 0.1f,1.0f };
		scoreSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	}


	/*===============================================================//
		 　　					コンボ
	//===============================================================*/
	combo_ = ComboManager::GetInstance()->GetMaxCombo();
	for (int i = 0; i < 10; ++i) {
		comboSprites_[i] = std::make_unique<Sprite>();
		comboSprites_[i].reset(Sprite::Create(scoreTh));
		comboSprites_[i]->Initialize();
		comboSprites_[i]->translate_ = {  };
		comboSprites_[i]->scale_ = { 0.07f, 0.5f };
		comboSprites_[i]->uvScale_ = { 0.1f,1.0f };
		comboSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	}


	/*===============================================================//
	 　　						敵倒
	//===============================================================*/

	count_ = CountManager::GetInstance()->GetEnemyCount();
	for (int i = 0; i < 10; ++i) {
		countSprites_[i] = std::make_unique<Sprite>();
		countSprites_[i].reset(Sprite::Create(scoreTh));
		countSprites_[i]->Initialize();
		countSprites_[i]->translate_ = {  };
		countSprites_[i]->scale_ = { 0.07f, 0.5f };
		countSprites_[i]->uvScale_ = { 0.1f,1.0f };
		countSprites_[i]->uvTranslate_ = { i * 0.1f, 0.0f };
	}

}

void ResultScene::Update()
{
#ifdef _DEBUG


	if (Input::GetInstance()->IsKeyTriggered(DIK_TAB) ||
		Input::GetInstance()->IsPadTriggered(PadButton::iPad_A))
	{
		SceneManager::GetInstance()->ReserveScene("Title");
	}

	ImGui::Begin("ResultScene");
	ImGui::DragFloat("Score", &resultSprite_[3]->rotate_);
	ImGui::End();

#endif // _DEBUG


	ScoreManager::GetInstance()->Update();
	ComboManager::GetInstance()->Update();
	CountManager::GetInstance()->ImGui();

}

void ResultScene::Draw()
{
	Sprite::PreDraw();
	for (uint32_t i = 0; i < ms_; ++i)
	{
		resultSprite_[i]->Draw();
	}

	DrawScore();
	DrawCombo();
	DrawCountEnemy();

}

void ResultScene::DrawScore()
{
	std::string scoreStr = std::to_string(score_); // スコアを文字列に変換
	float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
	float x = 700.0f - (scoreStr.size() * digitWidth); // スコア全体の右端を基準に調整
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
		Sprite::PreDraw();
		sprite->Draw();

		x += digitWidth; // 次の桁の位置に移動
	}
}

void ResultScene::DrawCountEnemy()
{
	std::string scoreStr = std::to_string(count_); // スコアを文字列に変換
	float digitWidth = 30.0f; // 各桁の幅（x方向の移動量）
	float x = 700.0f - (scoreStr.size() * digitWidth); // スコア全体の右端を基準に調整
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


