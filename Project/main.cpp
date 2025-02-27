#include "Core/WinApp/WinApp.h"
#include "Core/DirectX/DXCommon.h"
#include "Systems/Input/Input.h"
#include "ResourceManagement/TextureManager/TextureManager.h"
#include "Rendering/Model/ModelManager.h"
#include "Rendering/Sprite/Sprite.h"
#include "Rendering/LineDrawer/LineDrawer.h"
#include "ResourceManagement/SRVManager.h"
#include "Core/DirectX/PSOManager.h"
#include "UI/ImGuiManager/ImGuiManager.h"
#include "Framework/Particle/ParticleManager.h"
#include "Systems/Utility/RandomGenerator.h"
#include "Systems/Audio/AudioSystem.h"
#include "Systems/Time/Time.h"

/*-----シーン-----*/
#include "Framework/eScene/SceneManager.h"
#include "Framework/eScene/SampleScene.h"
#include "Framework/eScene/ParticleTestScene.h"

#include "Source/Scene/TitleScene.h"
#include "Source/Scene/GameScene.h"
#include "Source/Scene/ResultScene.h"
#include "Source/Scene/LoadScene.h"
#include "Source/Scene/SceneTransition.h"
/*---------------*/


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* winApp = WinApp::GetInstance();
	winApp->Initilize(L"2304_プラマイショット");

	DXCommon* dxCommon = DXCommon::GetInstance();
	dxCommon->Initialize(winApp, WinApp::kWindowWidth_, WinApp::kWindowHeight_);

	SRVManager* srvManager = SRVManager::GetInstance();
	srvManager->Initialize();
	PSOManager::GetInstance()->Initialize();


	std::unique_ptr<ImGuiManager> imguiManager = std::make_unique <ImGuiManager>();
	imguiManager->Initialize();

	ParticleManager* particle = ParticleManager::GetInstance();
	particle->Initialize();


	TextureManager::GetInstance()->Initialize();
	TextureManager::GetInstance()->Load("white.png");
	TextureManager::GetInstance()->Load("cube.jpg");
	TextureManager::GetInstance()->Load("uvChecker.png");

	Sprite::StaticInitialize(WinApp::kWindowWidth_, WinApp::kWindowHeight_);
	ModelManager::GetInstance()->Initialize();
    LightingSystem::GetInstance()->Initialize();

	LineDrawer* lineDrawer = LineDrawer::GetInstance();
	lineDrawer->Initialize();

	AudioSystem* audio_ = AudioSystem::GetInstance();
	audio_->Initialize();

	Input* input = Input::GetInstance();
	input->Initilize(winApp);

	JsonHub::GetInstance()->Initialize("Resources/Data/");

	//SceneManager::RegisterScene("Sample", SampleScene::Create);
	SceneManager::RegisterScene("Title", TitleScene::Create);
	SceneManager::RegisterScene("Game", GameScene::Create);
	SceneManager::RegisterScene("Result", ResultScene::Create);
    //SceneManager::RegisterScene("Load", LoadScene::Create);
	SceneManager::GetInstance()->SetTransition(std::make_unique<SceneTransition>());

#ifdef _DEBUG
	SceneManager::GetInstance()->Initialize("Result");
#else
    SceneManager::GetInstance()->Initialize("Title");
#endif // _DEBUG



	Time::Initialize();

	///
	/// メインループ
	///
	// ウィンドウのｘボタンが押されるまでループ
	while (!winApp->ProcessMessage())
	{
		imguiManager->Begin();

		///
		/// 更新処理ここから
		///

		SceneManager::GetInstance()->Update();
		//gameScene->Update();

		///
		/// 更新処理ここまで
		///

		dxCommon->PreDraw();
		srvManager->PreDraw();

		///
		/// 描画ここから
		///

		SceneManager::GetInstance()->Draw();
		//gameScene->Draw();


		///
		/// 描画ここまで
		///
		imguiManager->End();
		imguiManager->Draw();

		dxCommon->PostDraw();

		SceneManager::ChangeScene();
	}
	imguiManager->Finalize();

	winApp->Finalize();


	return 0;
}

