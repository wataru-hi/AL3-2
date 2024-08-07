#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles
{public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	bool IsFinished() const { return isFinished_;}
private:
	static inline const uint32_t kNumParticles = 8; 
	static inline const float kDuration = 0.5f;
	static inline const float kSpeed = 0.1f;
	static inline const float kAngleUnit = 2.0f * 3.14f / kNumParticles;

	Model* model_ = nullptr;
	//WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	std::array<WorldTransform, kNumParticles> worldTransforms_;
	bool isFinished_ = false;
	float counter_ = 0.0f;
	ObjectColor objectColor_;
	Vector4 color_;
};
