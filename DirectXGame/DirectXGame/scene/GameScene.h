#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

enum class Phase 
{
	kPlay,	//ゲームプレイ
	kDeath,	//デス演出
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

//getter
	bool isFinished() const { return finished; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 自キャラ
	Player* player_ = nullptr;
	//敵キャラ
	std::list<Enemy*> enemies_;
	// モデルデータ
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelDeathParticle_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform worldTransformSkydome_;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	MapChipField* mapChipField_;
	CameraController* cameraController = nullptr;
	DeathParticles* deathparticles_ = nullptr;
	Phase phase_;
	bool finished = false;
	uint32_t voiceHandle_ = 0;
	uint32_t soundHandle_ = 0;

	void GenerateBlocks();
	void CheckAllCollisions();
	void ChangePhase();
};
