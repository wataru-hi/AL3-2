#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include <cstdint>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete player_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	// enemies_ をクリア
	enemies_.clear();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}
	delete modelPlayer_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController;
	delete deathparticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	phase_ = Phase::kPlay;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle");

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 16);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	for (int32_t i = 0; i < 4; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12 + i, 16);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 天球の初期化
	worldTransformSkydome_.Initialize();
	
	GenerateBlocks();

	//音声再生
	soundHandle_ = audio_->LoadWave("royalPalaceExploration.wav");
	voiceHandle_ = audio_->PlayWave(soundHandle_, true);

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController->SetMovableArea(cameraArea);
}

void GameScene::Update() {

	switch (phase_) {
	case Phase::kPlay:

		worldTransformSkydome_.UpdateMatrix();

		// 自キャラの更新
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		#ifdef _DEBUG
		if (input_->TriggerKey(DIK_0)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
		#endif
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.matView = cameraController->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();
		}

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->UpdateMatrix();
			}
		}

		cameraController->Update();

		// すべての当たり判定を行う
		CheckAllCollisions();

		ChangePhase();

		break;
	case Phase::kDeath:

		worldTransformSkydome_.UpdateMatrix();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathparticles_) {
			deathparticles_->Update();
		}

		cameraController->Update();

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->UpdateMatrix();
			}
		}

		if (deathparticles_ && deathparticles_->IsFinished())
		{
			finished = true;
		}

		break;
	}

//	worldTransformSkydome_.UpdateMatrix();
//
//	// 自キャラの更新
//	player_->Update();
//
//	for (Enemy* enemy : enemies_) {
//		enemy->Update();
//	}
//
//	// すべての当たり判定を行う
//	CheckAllCollisions();
//
//	if (deathparticles_) {
//		deathparticles_->Update();
//	}
//
//#ifdef _DEBUG
//	if (input_->TriggerKey(DIK_0)) {
//		// フラグをトグル
//		isDebugCameraActive_ = !isDebugCameraActive_;
//	}
//#endif
//	if (isDebugCameraActive_) {
//		debugCamera_->Update();
//		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
//		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
//		// ビュープロジェクションの転送
//		viewProjection_.TransferMatrix();
//	} else {
//		viewProjection_.matView = cameraController->GetViewProjection().matView;
//		viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
//		// ビュープロジェクションの転送
//		viewProjection_.TransferMatrix();
//	}
//
//	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
//		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
//			if (!worldTransformBlock)
//				continue;
//
//			worldTransformBlock->UpdateMatrix();
//		}
//	}
//
//	cameraController->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 天球の描画
	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	// 自キャラの描画
	player_->Draw();

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (deathparticles_) {
		deathparticles_->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region プレイヤーと敵の当たり判定
	{
		AABB aabb1, aabb2;

		aabb1 = player_->getAABB();

		for (Enemy* enemy : enemies_) {
			aabb2 = enemy->getAABB();

			if (IsCollision(aabb1, aabb2)) {
				player_->OnCollision(enemy);

				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:

		if (player_->IsDead()) {
			phase_ = Phase::kDeath;
			const Vector3& deathParticlesPosition = player_->GetPosition();
		
		deathparticles_ = new DeathParticles;
	deathparticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);
		}

		break;
	case Phase::kDeath:


		break;
	}
}
