#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <array>

class DeathParticles
{
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	static inline const uint32_t kNumParticles = 8;	//パーティクルの数
	static inline const float kDutation = 3.0f;		//存在時間(秒)
	static inline const float kSpeed = 20.0f;		//移動の速さ
	static inline const float kAngelUnit = 2.0f * 3.14f / kNumParticles;	//一個分の角度


	Model* model_ = nullptr;
	std::array<WorldTransform, kNumParticles> worldTransfroms_;
	ViewProjection* viewProjection_ = nullptr;

	bool idFinished_ = false;	//終了フラグ
	float counter_ = 0.0f;	//経過時間カウント
};

